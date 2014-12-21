package com.personal.bourdi_b.desktopmousecontroller;

import android.app.Activity;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;

enum SideClickType {
    LEFT_CLICK,
    RIGHT_CLICK
}

public class MainActivity extends Activity {
    private final BroadcastMessageQueue _queue = new BroadcastMessageQueue();
    private Thread _networkThread;
    private NetworkRunner _networkRunner;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);

        _networkRunner = new NetworkRunner(getApplicationContext(), _queue);
        _networkThread = new Thread(_networkRunner, "_NetworkThread_");

        View touchView = findViewById(R.id.touchView);
        touchView.setOnTouchListener(new InputSurfaceListener(_queue));
        Button btLeftClick = (Button) findViewById(R.id.btLeftClick);
        Button btRightClick = (Button) findViewById(R.id.btRightClick);
        btLeftClick.setOnTouchListener(new MouseDownListener(SideClickType.LEFT_CLICK, _queue));
        btRightClick.setOnTouchListener(new MouseDownListener(SideClickType.RIGHT_CLICK, _queue));

        if (!_networkThread.isAlive()) {
            _networkThread.start();
        }
    }

    @Override
    protected void onDestroy() {
        try {
            // We unlock the thread (which is stuck in queue reading).
            _queue.put(BroadcastMessageFactory.createPoisonMessage());
            _networkThread.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        super.onDestroy();
    }
}

class InputSurfaceListener implements View.OnTouchListener {

    private final BroadcastMessageQueue _queue;
    private MyPoint _fromPoint = null;

    public InputSurfaceListener(BroadcastMessageQueue queue) {
        _queue = queue;
    }

    @Override
    public boolean onTouch(View view, MotionEvent motionEvent) {
        int action = motionEvent.getActionMasked();

        switch (action) {
            case MotionEvent.ACTION_DOWN:
                _fromPoint = new MyPoint(((int) motionEvent.getX()), (int) motionEvent.getY());
                return true;
            case MotionEvent.ACTION_MOVE:
                MyPoint toPoint = new MyPoint((int) motionEvent.getX(), (int) motionEvent.getY());
                if (toPoint != _fromPoint) {
                    MyDeltaPoint deltaPoint = new MyDeltaPoint(_fromPoint, toPoint);
                    BroadcastMessage message = BroadcastMessageFactory
                            .createMoveMessage(deltaPoint);
                    try {
                        _queue.put(message);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
                _fromPoint = new MyPoint(toPoint);
                return true;
        }
        return false;
    }
}

class MouseDownListener implements View.OnTouchListener {

    private static final int NUMBER_MESSAGES_TO_SEND = 30;
    private final BroadcastMessageQueue _queue;
    private final SideClickType _type;

    public MouseDownListener(SideClickType type, BroadcastMessageQueue queue) {
        _queue = queue;
        _type = type;
    }

    @Override
    public boolean onTouch(View view, MotionEvent motionEvent) {
        BroadcastMessage message = null;
        switch (motionEvent.getAction()) {
            case MotionEvent.ACTION_DOWN:
                message = onDownClick();
                break;
            case MotionEvent.ACTION_UP:
                message = onUpClick();
                break;
        }
        pushMessage(message);
        return true;
    }

    private BroadcastMessage onUpClick() {
        BroadcastMessage message = null;
        switch (_type) {
            case LEFT_CLICK:
                message = BroadcastMessageFactory.createLeftUpClickMessage();
                break;
            case RIGHT_CLICK:
                message = BroadcastMessageFactory.createRightUpClickMessage();
                break;
        }
        return message;
    }

    private BroadcastMessage onDownClick() {
        BroadcastMessage message = null;
        switch (_type) {
            case LEFT_CLICK:
                message = BroadcastMessageFactory.createLeftDownClickMessage();
                break;
            case RIGHT_CLICK:
                message = BroadcastMessageFactory.createRightDownClickMessage();
                break;
        }
        return message;
    }

    private void pushMessage(BroadcastMessage message) {
        if (message != null) {
            try {
                for (int i = 0; i < NUMBER_MESSAGES_TO_SEND; ++i) {
                    _queue.put(message);
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}
