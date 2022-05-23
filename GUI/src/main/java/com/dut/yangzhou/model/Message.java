package com.dut.yangzhou.model;

public class Message {

    private int id;

    private String toIp;

    private int toPort;

    private String content;

    private int flag;

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getToIp() {
        return toIp;
    }

    public void setToIp(String toIp) {
        this.toIp = toIp;
    }

    public int getToPort() {
        return toPort;
    }

    public void setToPort(int toPort) {
        this.toPort = toPort;
    }

    public String getContent() {
        return content;
    }

    public void setContent(String content) {
        this.content = content;
    }

    public int getFlag() {
        return flag;
    }

    public void setFlag(int flag) {
        this.flag = flag;
    }

    @Override
    public String toString() {
        return "Message{" +
                "id=" + id +
                ", toIp='" + toIp + '\'' +
                ", toPort=" + toPort +
                ", content='" + content + '\'' +
                ", flag=" + flag +
                '}';
    }
}
