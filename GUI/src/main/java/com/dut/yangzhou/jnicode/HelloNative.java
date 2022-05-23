package com.dut.yangzhou.jnicode;

public class HelloNative {

    public native void greeting(int a, int b);

    static {
        System.loadLibrary("hellonative");

    }

    public static void main(String[] args) {

        new HelloNative().greeting(1, 2);

    }

}
