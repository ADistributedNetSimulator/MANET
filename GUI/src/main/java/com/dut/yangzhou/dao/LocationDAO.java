package com.dut.yangzhou.dao;

import com.dut.yangzhou.model.Location;
import org.apache.ibatis.annotations.*;

import java.util.ArrayList;

@Mapper
public interface LocationDAO {
    // 注意空格
    String TABLE_NAME = " location ";
    String INSERT_FIELDS = " x, y, z ";
    String SELECT_FIELDS = " id, " + INSERT_FIELDS;

    @Select({"select * from location"})
    ArrayList<Location> getAllLocation();

    @Select({"select * from location where id=#{id}"})
    Location getById(int id);

    @Select({"select id from location"})
    ArrayList<Integer> getAllNodes();

    @Insert({"insert into ", TABLE_NAME, "(", SELECT_FIELDS,
            ") values (#{id},#{x},#{y},#{z} )"})

    int insertLocation(Location location);

    @Update({"update ", TABLE_NAME, " set x=#{x},y=#{y},z=#{z}"})
    int updateLocation(Location location);

    @Update({"update ", TABLE_NAME, " set x=#{x},y=#{y},z=#{z} where id=#{id}"})
    int updateXYZ(@Param("id")int id, @Param("x")int x, @Param("y")int y, @Param("z")int z);

    @Delete({"delete from location where id = #{id}"})
    void deleteLocationById(@Param("id")int id);
}
