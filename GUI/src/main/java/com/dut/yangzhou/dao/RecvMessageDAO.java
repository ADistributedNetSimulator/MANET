package com.dut.yangzhou.dao;


import com.dut.yangzhou.model.RecvMessage;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Select;

import java.util.ArrayList;

@Mapper
public interface RecvMessageDAO {

    @Select({"select * from recv_message where id=#{id}"})
    RecvMessage getById(int id);

    @Select({"select * from recv_message"})
    ArrayList<RecvMessage> getAll();
}
