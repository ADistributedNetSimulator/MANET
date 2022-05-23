package com.dut.yangzhou.controller;

import com.dut.yangzhou.model.State;
import com.dut.yangzhou.service.SettingService;
import com.dut.yangzhou.service.StateService;
import com.dut.yangzhou.util.CommunityUtil;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;

import java.io.IOException;


@Controller
public class StateController {

    private static Logger logger = LoggerFactory.getLogger(StateController.class);

    @Autowired
    private StateService stateService;

    @RequestMapping(path = "/del")
    @ResponseBody
    public String state(int id) {
        State state = new State();
        state.setId(id);
        state.setState(0);
        stateService.updateState(state);

        return CommunityUtil.getJSONString(0, "OK");
    }

}
