package com.dut.yangzhou.service;

import com.dut.yangzhou.dao.*;
import com.dut.yangzhou.model.Ipnode;
import com.dut.yangzhou.model.Message;
import com.dut.yangzhou.model.RecvMessage;
import com.dut.yangzhou.model.State;
import com.dut.yangzhou.util.CommunityUtil;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Isolation;
import org.springframework.transaction.annotation.Propagation;
import org.springframework.transaction.annotation.Transactional;

@Service
public class StateService {

    private static final Logger logger = LoggerFactory.getLogger(StateService.class);

    @Autowired
    private StateDAO stateDAO;

    @Autowired
    private LocationDAO locationDAO;

    @Transactional(isolation = Isolation.READ_COMMITTED, propagation = Propagation.REQUIRED)
    public int updateState(State state) {
        locationDAO.deleteLocationById(state.getId());
        return stateDAO.updateState(state);
    }
}
