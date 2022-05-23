package com.dut.yangzhou.dao;

import com.dut.yangzhou.model.Delay;
import com.dut.yangzhou.model.Ipnode;
import com.dut.yangzhou.model.Location;
import org.apache.ibatis.annotations.*;

import java.util.ArrayList;

@Mapper
public interface DelayDAO {

    // SELECT * from delay where id = (select max(id) from delay where sourceip = "172.17.0.5")
    // SELECT max(delay) from delay where sourceip = "172.17.0.5" and  destinationip = "172.17.0.4"
    @Select({"SELECT max(delay) from delay where sourceip = #{sourceip} and destinationip = #{destinationIp}"})
    Double getBySourceip(@Param("sourceip") String sourceip, @Param("destinationIp")String destinationIp);

    @Select({"SELECT * from delay where sourceip = #{sourceip} and destinationip = #{destinationIp}"})
    ArrayList<Delay> isExist(@Param("sourceip") String sourceip, @Param("destinationIp")String destinationIp);

    @Select({"SELECT avg(delay) from delay where sourceip = #{sourceip} and destinationip = #{destinationIp}"})
    Double getByDelayAVG(@Param("sourceip") String sourceip, @Param("destinationIp")String destinationIp);

    @Select({"SELECT count(*) from delay where destinationip = #{destinationIp}"})
    Integer getPacketCount(@Param("destinationIp")String destinationIp);
}
