package com.dut.yangzhou.model;

public class PoorChannel {
    private int id;
    private int poorChannel;

    public PoorChannel() {
    }

    public PoorChannel(int id) {
        this.id = id;
    }


    public PoorChannel(int id, int poorChannel) {
        this.id = id;
        this.poorChannel = poorChannel;
    }

    public int getId() {
        return id;
    }

    public int getPoorChannel() {
        return poorChannel;
    }

    public void setId(int id) {
        this.id = id;
    }

    public void setPoorChannel(int poorChannel) {
        this.poorChannel = poorChannel;
    }

    @Override
    public String toString() {
        return "PoorChannel{" +
                "id=" + id +
                ", poorChannel=" + poorChannel +
                '}';
    }

}
