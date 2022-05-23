package com.dut.yangzhou.service;

import com.dut.yangzhou.dao.IpnodeDAO;
import com.dut.yangzhou.dao.MessageDAO;
import com.dut.yangzhou.dao.RecvMessageDAO;
import com.dut.yangzhou.model.Ipnode;
import com.dut.yangzhou.model.Message;
import com.dut.yangzhou.model.RecvMessage;
import com.dut.yangzhou.util.CommunityUtil;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

@Service
public class MessageService {

    private static final Logger logger = LoggerFactory.getLogger(MessageService.class);

    @Autowired
    private MessageDAO messageDAO;

    @Autowired
    private IpnodeDAO ipnodeDAO;

    @Autowired
    private RecvMessageDAO recvMessageDAO;

    public int insertMessage(Message message) {
        return messageDAO.insertMessage(message);
    }

    public int insertMessage(int fromId, int toId, String content) throws InterruptedException {
        if (messageDAO.getById(fromId) == null) {
            // 插入
            Ipnode ipnode = ipnodeDAO.getById(toId);
            Message message = new Message();
            message.setId(fromId);
            message.setToIp(ipnode.getIp());
            message.setToPort(ipnode.getPort());
            message.setContent(content);
            messageDAO.insertMessage(message);
        } else {
            // 更新
            Ipnode ipnode = ipnodeDAO.getById(toId);
            Message message = new Message();
            message.setId(fromId);
            message.setToIp(ipnode.getIp());
            message.setToPort(ipnode.getPort());
            message.setContent(content);
            messageDAO.updateMessage(message);
        }

        // 监听数据库
        int flag = CommunityUtil.flagMap.get(toId);
        int flag1 = CommunityUtil.flagMap.get(fromId);
        int res = 0;
        while (true) {
            Thread.sleep(1500);
            RecvMessage recvMessage = recvMessageDAO.getById(toId);
            RecvMessage recvMessage1 = recvMessageDAO.getById(fromId);

            if (flag1 == recvMessage1.getFlag()) {
                System.out.println("flag1没有变化");
            } else {
                System.out.println("flag变化");
                CommunityUtil.flagMap.put(toId, recvMessage1.getFlag());
                res = 1; // 说明信息发送失败
                break;
            }

            if (flag == recvMessage.getFlag()) {
                System.out.println("flag没有变化");
            } else {
                System.out.println("flag变化");
                CommunityUtil.flagMap.put(toId, recvMessage.getFlag());
                res = 2;
                break;
            }
        }
        return res;
    }
}
