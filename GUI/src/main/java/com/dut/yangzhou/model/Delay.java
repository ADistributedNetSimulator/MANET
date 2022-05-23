package com.dut.yangzhou.model;

public class Delay {

    private int id;

    private String sourceip;

    private String destinationip;

    private String packet;

    private double delay;

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getSourceip() {
        return sourceip;
    }

    public void setSourceip(String sourceip) {
        this.sourceip = sourceip;
    }

    public String getDestinationip() {
        return destinationip;
    }

    public void setDestinationip(String destinationip) {
        this.destinationip = destinationip;
    }

    public String getPacket() {
        return packet;
    }

    public void setPacket(String packet) {
        this.packet = packet;
    }

    public double getDelay() {
        return delay;
    }

    public void setDelay(double delay) {
        this.delay = delay;
    }

    @Override
    public String toString() {
        return "Delay{" +
                "id=" + id +
                ", sourceip='" + sourceip + '\'' +
                ", destinationip='" + destinationip + '\'' +
                ", packet='" + packet + '\'' +
                ", delay=" + delay +
                '}';
    }
}
