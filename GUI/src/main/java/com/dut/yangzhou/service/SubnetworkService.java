package com.dut.yangzhou.service;

import com.dut.yangzhou.dao.SubnetworkDao;
import com.dut.yangzhou.model.Ipnode;
import com.dut.yangzhou.model.SubNet;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;

@Service
public class SubnetworkService {

    private static final Logger logger = LoggerFactory.getLogger(SubnetworkService.class);

    @Autowired
    private SubnetworkDao subnetworkDao;


    public Map<String, Object> getLastSub() {
        Map<String, Object> res = new HashMap<>();

        // 获取数据库所有的节点
        ArrayList<SubNet> list = subnetworkDao.getLastSub();

        // 获取xData的数据
        ArrayList<String> xData = new ArrayList<>();
        for (int i = 0; i < list.size(); i++) {
            xData.add(list.get(i).getTime());
        }
        res.put("xData", xData);

        // 获取yData的数据
        ArrayList<Integer> yData = new ArrayList<>();
        for (int i = 0; i < list.size(); i++) {

            int count = list.get(i).getCount();
            yData.add(count);
        }
        res.put("yData", yData);
//        System.out.println("------getPacket结束------");

        return res;
    }

}
