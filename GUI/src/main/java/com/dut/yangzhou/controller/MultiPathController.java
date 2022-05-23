package com.dut.yangzhou.controller;

import com.dut.yangzhou.model.MultiPath;
import com.dut.yangzhou.service.MultiPathService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.GetMapping;

@Controller
public class MultiPathController {
    @Autowired
    MultiPathService multiPathService;
    @GetMapping(value = "multiPathTransferGet")
    public String MPinsert(MultiPath multiPath){
        multiPathService.UpdateMP(multiPath);
        return "redirect:/index";
    }
}
