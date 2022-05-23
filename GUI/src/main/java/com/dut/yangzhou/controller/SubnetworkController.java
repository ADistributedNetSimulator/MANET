package com.dut.yangzhou.controller;

import com.dut.yangzhou.service.SubnetworkService;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;

import java.util.Map;


@Controller
public class SubnetworkController {

    private static Logger logger = LoggerFactory.getLogger(SubnetworkController.class);

    @Autowired
    private SubnetworkService subnetworkService;

    @RequestMapping(path = "/getSub")
    @ResponseBody
    public Map<String, Object> getSubNet() {

        Map<String, Object> res = subnetworkService.getLastSub();
//        ArrayList<SubNet> subNets = subnetworkService.getLastSub();

        return res;
    }

}
