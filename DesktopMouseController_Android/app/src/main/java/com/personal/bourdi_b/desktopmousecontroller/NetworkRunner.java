package com.personal.bourdi_b.desktopmousecontroller;

import android.content.Context;
import android.net.DhcpInfo;
import android.net.wifi.WifiManager;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;

public class NetworkRunner implements Runnable {

    static private final int LOCAL_PORT = 10501;
    static private final int SERVER_PORT = 10500;

    private final Context _context;
    private final BroadcastMessageQueue _queue;
    private DatagramSocket _clientSocket = null;
    private InetAddress _IPAddress = null;

    public NetworkRunner(Context context, BroadcastMessageQueue queue) {
        _context = context;
        _queue = queue;
    }

    public void run() {
        if (!prepare()) {
            return;
        }

        while (true) {
            try {
                BroadcastMessage message = _queue.take();
                // use of Poison Pill Shutdown
                if (isPoisonMessage(message)) {
                    break;
                }
                //Log.d("TOUCH_TAG", "We send sth:" + message);
                sendPacket(message.toString());
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        _clientSocket.close();
    }

    Boolean prepare() {
        if (_clientSocket == null || _clientSocket.isClosed()) {
            if (!initializeSocket()) {
                return false;
            }
            _IPAddress = getBroadcastAddress();
            if (_IPAddress == null) {
                _clientSocket.close();
                _clientSocket = null;
                return false;
            }
        }
        return true;
    }

    Boolean initializeSocket() {
        try {
            _clientSocket = new DatagramSocket(LOCAL_PORT);
        } catch (SocketException e) {
            e.printStackTrace();
            return false;
        }
        return true;
    }

    InetAddress getBroadcastAddress() {
        try {
            WifiManager wifi = (WifiManager) _context.getSystemService(Context.WIFI_SERVICE);
            DhcpInfo dhcp = wifi.getDhcpInfo();
            if (dhcp == null) {
                throw new IOException("DHCP is null");
            }

            int broadcast = (dhcp.ipAddress & dhcp.netmask) | ~dhcp.netmask;
            byte[] quads = new byte[4];
            for (int k = 0; k < 4; k++) {
                quads[k] = (byte) ((broadcast >> k * 8) & 0xFF);
            }
            return InetAddress.getByAddress(quads);
        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }

    Boolean isPoisonMessage(BroadcastMessage message) {
        return message.toString().equals(BroadcastMessageFactory.POISON_MESSAGE);
    }

    void sendPacket(String str) {
        DatagramPacket packet = new DatagramPacket(str.getBytes(), str.getBytes().length,
                                                   _IPAddress, SERVER_PORT);
        try {
            _clientSocket.send(packet);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
