package com.dut.yangzhou.dao;

import com.dut.yangzhou.model.Setting;
import org.apache.ibatis.annotations.Insert;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Select;
import org.apache.ibatis.annotations.Update;

@Mapper
public interface SettingDAO {
    // 注意空格
    String TABLE_NAME = " setting ";
    String INSERT_FIELDS = " bandwidth ";
    String SELECT_FIELDS = " id, " + INSERT_FIELDS;

    @Update({"update setting set bandwidth=#{bandwidth} where id=#{id}"})
    int updateSetting(Setting setting);


    @Select({"select * from setting where id=#{id}"})
    Setting getById(int id);

    @Insert({"insert into ", TABLE_NAME, "(", SELECT_FIELDS,
            ") values (#{id},#{bandwidth})"})
    int insertSetting(Setting setting);

}
