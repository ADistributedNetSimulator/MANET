package com.dut.yangzhou.dao;

import com.dut.yangzhou.model.Ipnode;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Select;

@Mapper
public interface IpnodeDAO {
    // 注意空格
    String TABLE_NAME = " ipnode ";
    String INSERT_FIELDS = " ip,port ";
    String SELECT_FIELDS = " id, " + INSERT_FIELDS;

    @Select({"select * from ipnode where id=#{id}"})
    Ipnode getById(int id);
}
