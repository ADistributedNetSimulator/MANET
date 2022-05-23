package com.dut.yangzhou.dao;

import com.dut.yangzhou.model.Location;
import com.dut.yangzhou.model.PoorChannel;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;
import org.apache.ibatis.annotations.Select;
import org.apache.ibatis.annotations.Update;


@Mapper
public interface PoorChannelDAO{
    @Update({"update poor_channel set poor_channel=#{poorChannel} where id=#{id}"})
    int updatepoorChannel(PoorChannel abd);
}
