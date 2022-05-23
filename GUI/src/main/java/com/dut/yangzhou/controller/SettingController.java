package com.dut.yangzhou.controller;

import com.dut.yangzhou.service.SettingService;
import com.dut.yangzhou.util.CommunityUtil;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;

import java.io.IOException;


@Controller
public class SettingController {

    private static Logger logger = LoggerFactory.getLogger(SettingController.class);

    @Autowired
    private SettingService settingService;

    @RequestMapping(path = "/init")
    @ResponseBody
    public String init(int id, double bandwidth, boolean nodeinit) throws IOException, InterruptedException {

        settingService.init(id, bandwidth, nodeinit);

        return CommunityUtil.getJSONString(0, "Initialization succeeded");
    }
}
