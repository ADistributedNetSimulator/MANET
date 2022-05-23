package com.dut.yangzhou.service;

import com.dut.yangzhou.dao.MultiPathDAO;
import com.dut.yangzhou.model.MultiPath;
import com.dut.yangzhou.model.PoorChannel;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Isolation;
import org.springframework.transaction.annotation.Propagation;
import org.springframework.transaction.annotation.Transactional;

@Service
public class MultiPathService {
    @Autowired
    private MultiPathDAO multiPathDAO;
    public int  UpdateMP(MultiPath multiPath){
        int poorChanel=multiPathDAO.insertMP(multiPath);
        return  poorChanel;
    }
}
