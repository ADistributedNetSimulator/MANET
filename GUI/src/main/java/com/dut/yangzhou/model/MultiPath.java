package com.dut.yangzhou.model;

public class MultiPath {
    private int sourceId;
    private int destinationId;
    private String multiPath;
    private String multiPathMessage;
    private int messageFlag;

    public MultiPath(int sourceId, int destinationId, String multiPath, String multiPathMessage, int messageFlag) {
        this.sourceId = sourceId;
        this.destinationId = destinationId;
        this.multiPath = multiPath;
        this.multiPathMessage = multiPathMessage;
        this.messageFlag = messageFlag;
    }

    public MultiPath() {
    }


    public int getSourceId() {
        return sourceId;
    }

    public void setSourceId(int sourceId) {
        this.sourceId = sourceId;
    }

    public int getDestinationId() {
        return destinationId;
    }

    public void setDestinationId(int destinationId) {
        this.destinationId = destinationId;
    }

    public String getMultiPath() {
        return multiPath;
    }

    public void setMultiPath(String multiPath) {
        this.multiPath = multiPath;
    }

    public String getMultiPathMessage() {
        return multiPathMessage;
    }

    public void setMultiPathMessage(String multiPathMessage) {
        this.multiPathMessage = multiPathMessage;
    }

    public int getMessageFlag() {
        return messageFlag;
    }

    public void setMessageFlag(int messageFlag) {
        this.messageFlag = messageFlag;
    }

    @Override
    public String toString() {
        return "MultiPath{" +
                "sourceId=" + sourceId +
                ", destinationId=" + destinationId +
                ", multiPath='" + multiPath + '\'' +
                ", multiPathMessage='" + multiPathMessage + '\'' +
                ", messageFlag=" + messageFlag +
                '}';
    }
}
