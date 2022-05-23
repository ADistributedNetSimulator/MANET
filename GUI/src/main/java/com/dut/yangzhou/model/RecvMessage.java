package com.dut.yangzhou.model;

public class RecvMessage {

    private int id;

    private String recvMessage;

    private int flag;

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getRecvMessage() {
        return recvMessage;
    }

    public void setRecvMessage(String recvMessage) {
        this.recvMessage = recvMessage;
    }

    public int getFlag() {
        return flag;
    }

    public void setFlag(int flag) {
        this.flag = flag;
    }

    @Override
    public String toString() {
        return "RecvMessage{" +
                "id=" + id +
                ", recvMessage='" + recvMessage + '\'' +
                ", flag=" + flag +
                '}';
    }
}
