package com.dut.yangzhou.dao;

import com.dut.yangzhou.model.State;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;
import org.apache.ibatis.annotations.Select;
import org.apache.ibatis.annotations.Update;


@Mapper
public interface StateDAO {

    @Update({"update state set state = #{state} where id=#{id}"})
    int updateState(State state);

    @Select({"SELECT * from state where id = #{id}"})
    Integer getById(@Param("id")int id);

}
