package com.dut.yangzhou.dao;

import com.dut.yangzhou.model.SubNet;
import org.apache.ibatis.annotations.Insert;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Select;

import java.util.ArrayList;

@Mapper
public interface SubnetworkDao {

    @Select({"select * from subnetwork order by id desc limit 6"})
    ArrayList<SubNet> getLastSub();


    @Insert({"insert into subnetwork(count,time) values(#{count},#{time})"})
    int addSub(SubNet subNet);
}
