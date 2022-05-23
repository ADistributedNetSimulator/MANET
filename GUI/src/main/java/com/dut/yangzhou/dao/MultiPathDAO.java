package com.dut.yangzhou.dao;

import com.dut.yangzhou.model.Location;

import com.dut.yangzhou.model.MultiPath;
import org.apache.ibatis.annotations.Insert;
import org.apache.ibatis.annotations.Mapper;


@Mapper
public interface MultiPathDAO {
    @Insert({"insert into  multi_path_transfer(source_id,destination_id,multi_path,multi_path_message)  values (#{sourceId},#{destinationId},#{multiPath},#{multiPathMessage})"})
    int insertMP(MultiPath multiPath);
}
