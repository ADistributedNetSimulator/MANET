package com.dut.yangzhou.controller;

import com.dut.yangzhou.dao.PoorChannelDAO;
import com.dut.yangzhou.model.PoorChannel;
import com.dut.yangzhou.service.PoorChannelService;

//import com.dut.yangzhou.util.CommunityUtil;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;

import java.io.IOException;
@Controller
public class PoorChannelController {
    @Autowired
    PoorChannelService pc;
    @GetMapping("poorChannelGet")
    public String poorChannelGet(PoorChannel poorChannel){
        pc.UpdatePoorChanel(poorChannel);
        return "redirect:/index";
    }
}
