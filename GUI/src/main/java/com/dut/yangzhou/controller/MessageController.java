package com.dut.yangzhou.controller;

import com.dut.yangzhou.dao.RecvMessageDAO;
import com.dut.yangzhou.model.RecvMessage;
import com.dut.yangzhou.service.MessageService;
import com.dut.yangzhou.util.CommunityUtil;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;

@Controller
public class MessageController {

    private static Logger logger = LoggerFactory.getLogger(MessageController.class);

    @Autowired
    private MessageService messageService;

    @Autowired
    private RecvMessageDAO recvMessageDAO;

    @RequestMapping(path = "/send", method = RequestMethod.POST)
    @ResponseBody
    public String send(int fromId,int toId, String content) throws InterruptedException {
//        System.out.println("=======send========");
//        System.out.println(fromId);
//        System.out.println(toId);
//        System.out.println(content);
        int res = messageService.insertMessage(fromId, toId, content);
        if (res == 1) {
            RecvMessage message = recvMessageDAO.getById(fromId);
            return CommunityUtil.getJSONString(0, "Failed to send" + message.getRecvMessage());
        } else if (res == 2) {
            RecvMessage message = recvMessageDAO.getById(toId);
            return CommunityUtil.getJSONString(0, "Sent successfully" + message.getRecvMessage());
        } else {
            return CommunityUtil.getJSONString(0, "server error");
        }
    }
}
