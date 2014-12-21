package com.personal.bourdi_b.desktopmousecontroller;

public class BroadcastMessageFactory {

    public static final String POISON_MESSAGE = "TIME_TO_QUIT";

    public static BroadcastMessage createMoveMessage(MyDeltaPoint deltaPoint) {
        StringBuilder msgBuilder = new StringBuilder();
        msgBuilder.append("{ \"type\" : MOVE");
        msgBuilder.append(", \"from_x\" : ");
        msgBuilder.append(deltaPoint.fromPoint.x);
        msgBuilder.append(", \"from_y\" : ");
        msgBuilder.append(deltaPoint.fromPoint.y);
        msgBuilder.append(", \"to_x\" : ");
        msgBuilder.append(deltaPoint.toPoint.x);
        msgBuilder.append(", \"to_y\" : ");
        msgBuilder.append(deltaPoint.toPoint.y);
        msgBuilder.append("}");

        return new BroadcastMessage(msgBuilder.toString());
    }

    public static BroadcastMessage createLeftDownClickMessage() {
        StringBuilder msgBuilder = new StringBuilder();
        msgBuilder.append("{ \"type\" : LEFT_DOWN_CLICK }");
        return new BroadcastMessage(msgBuilder.toString());
    }

    public static BroadcastMessage createRightDownClickMessage() {
        StringBuilder msgBuilder = new StringBuilder();
        msgBuilder.append("{ \"type\" : RIGHT_DOWN_CLICK }");
        return new BroadcastMessage(msgBuilder.toString());
    }

    public static BroadcastMessage createLeftUpClickMessage() {
        StringBuilder msgBuilder = new StringBuilder();
        msgBuilder.append("{ \"type\" : LEFT_UP_CLICK }");
        return new BroadcastMessage(msgBuilder.toString());
    }

    public static BroadcastMessage createRightUpClickMessage() {
        StringBuilder msgBuilder = new StringBuilder();
        msgBuilder.append("{ \"type\" : RIGHT_UP_CLICK }");
        return new BroadcastMessage(msgBuilder.toString());
    }

    public static BroadcastMessage createPoisonMessage() {
        return new BroadcastMessage(POISON_MESSAGE);
    }

}
