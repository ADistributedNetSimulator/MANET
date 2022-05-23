package com.dut.yangzhou.dao;

import com.dut.yangzhou.model.Message;
import org.apache.ibatis.annotations.Insert;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Select;
import org.apache.ibatis.annotations.Update;

@Mapper
public interface MessageDAO {
    // 注意空格
    String TABLE_NAME = " message ";
    String INSERT_FIELDS = " toIp,toPort,content ";
    String SELECT_FIELDS = " id, " + INSERT_FIELDS;

    @Insert({"insert into ", TABLE_NAME, "(", SELECT_FIELDS,
            ") values (#{id},#{toIp},#{toPort},#{content})"})
    int insertMessage(Message message);

    @Update({"update ", TABLE_NAME, " set toIp=#{toIp},toPort=#{toPort},content=#{content} where id=#{id}"})
    int updateMessage(Message message);

    @Select({"select * from message where id=#{id}"})
    Message getById(int id);
}
