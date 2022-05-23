package com.dut.yangzhou.service;

import com.dut.yangzhou.controller.PoorChannelController;
import com.dut.yangzhou.dao.*;
import com.dut.yangzhou.model.Ipnode;
import com.dut.yangzhou.model.Message;
import com.dut.yangzhou.model.RecvMessage;
import com.dut.yangzhou.model.PoorChannel;
import com.dut.yangzhou.util.CommunityUtil;
import org.apache.ibatis.annotations.Update;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Isolation;
import org.springframework.transaction.annotation.Propagation;
import org.springframework.transaction.annotation.Transactional;

@Service
public class PoorChannelService {
    @Autowired
    private PoorChannelDAO poorChannelDAO;
    @Transactional(isolation = Isolation.READ_COMMITTED, propagation = Propagation.REQUIRED)
    public int  UpdatePoorChanel(PoorChannel pc){
        int poorChanel=poorChannelDAO.updatepoorChannel(pc);
        return  poorChanel;
    }

}
