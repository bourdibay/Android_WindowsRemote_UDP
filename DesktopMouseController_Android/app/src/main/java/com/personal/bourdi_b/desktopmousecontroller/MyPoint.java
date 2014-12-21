package com.personal.bourdi_b.desktopmousecontroller;

public class MyPoint {
    public final int x;
    public final int y;

    public MyPoint(int x, int y) {
        this.x = x;
        this.y = y;
    }

    public MyPoint(MyPoint point) {
        x = point.x;
        y = point.y;
    }

    @Override
    public boolean equals(Object obj) {
        if (obj == this) {
            return true;
        }
        if (!(obj instanceof MyPoint)) {
            return false;
        }
        MyPoint point = (MyPoint) obj;
        return point.x == x && point.y == y;
    }

    @Override
    public String toString() {
        return "Point(" + x + ", " + y + ")";
    }
}
