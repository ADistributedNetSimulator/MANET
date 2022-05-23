package com.dut.yangzhou.model;

import java.util.Date;

public class SubNet {

    private int id;

    private int count;

    private String time;

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getCount() {
        return count;
    }

    public void setCount(int count) {
        this.count = count;
    }

    public String getTime() {
        return time;
    }

    public void setTime(String time) {
        this.time = time;
    }

    @Override
    public String toString() {
        return "SubNet{" +
                "id=" + id +
                ", count=" + count +
                ", time=" + time +
                '}';
    }
}
