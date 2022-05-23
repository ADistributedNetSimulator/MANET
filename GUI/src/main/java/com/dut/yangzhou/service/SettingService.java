package com.dut.yangzhou.service;

import com.dut.yangzhou.dao.LocationDAO;
import com.dut.yangzhou.dao.SettingDAO;
import com.dut.yangzhou.model.Location;
import com.dut.yangzhou.model.Setting;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Isolation;
import org.springframework.transaction.annotation.Propagation;
import org.springframework.transaction.annotation.Transactional;

import java.io.*;


@Service
public class SettingService {
    private static final Logger logger = LoggerFactory.getLogger(SettingService.class);

    @Autowired
    private SettingDAO settingDAO;

    @Autowired
    private LocationDAO locationDAO;

    @Transactional(isolation = Isolation.READ_COMMITTED, propagation = Propagation.REQUIRED)
    public int init(int id, double bandwidth, boolean nodeinit) throws IOException {

        if (settingDAO.getById(id) == null) {
            // 插入节点
            Setting setting = new Setting();
            setting.setId(id);
            setting.setBandwidth(bandwidth);
            settingDAO.insertSetting(setting);
        } else {
            // 更改带宽
            Setting setting = new Setting();
            setting.setId(id);
            setting.setBandwidth(bandwidth);
            settingDAO.updateSetting(setting);
        }

        if (nodeinit) {
            System.out.println("从文件读取解析，持久化到数据库");
            // 从文件读取解析，持久化到数据库
            String fileName = "/Users/syd/Desktop/test.txt";
            File file = new File(fileName);
            FileReader fileReader = new FileReader(file);
            BufferedReader br = new BufferedReader(fileReader);

            int count = 0;

            String line = null;
            while ((line = br.readLine()) != null) {
                System.out.println(line);
                String[] values = line.split(",");

                // 判断是否已经存在该节点，如果存在就更新，不存在就插入。
                Location location = locationDAO.getById(Integer.parseInt(values[0]));
                if (location == null) {
                    location = new Location();
                    location.setId(Integer.parseInt(values[0]));
                    location.setX(Integer.parseInt(values[1]));
                    location.setY(Integer.parseInt(values[2]));
                    location.setZ(Integer.parseInt(values[3]));

                    count = locationDAO.insertLocation(location);
                } else {
                    location = new Location();
                    location.setId(Integer.parseInt(values[0]));
                    location.setX(Integer.parseInt(values[1]));
                    location.setY(Integer.parseInt(values[2]));
                    location.setZ(Integer.parseInt(values[3]));

                    locationDAO.updateLocation(location);
                }
            }
            br.close();
            return count;
        } else {
            return 0;
        }
    }
}
