package com.dut.yangzhou;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

import javax.annotation.PostConstruct;

@SpringBootApplication
public class YangzhouApplication {

//    class Task implements Runnable {
//        @Override
//        public void run() {
//            while (true) {
//                try {
//                    Thread.sleep(1000);
//                    System.out.println("=====");
//                } catch (InterruptedException e) {
//                    e.printStackTrace();
//                }
//            }
//        }
//    }
//
//
//    @PostConstruct
//    public void listen() {
//        Task task = new Task();
//        Thread thread = new Thread(task);
//        thread.start();
//        thread.start();
//    }

    public static void main(String[] args) {
        SpringApplication.run(YangzhouApplication.class, args);
    }

}
