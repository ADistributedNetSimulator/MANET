package com.dut.yangzhou.service;


import com.dut.yangzhou.dao.*;
import com.dut.yangzhou.model.*;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Isolation;
import org.springframework.transaction.annotation.Propagation;
import org.springframework.transaction.annotation.Transactional;

import java.text.SimpleDateFormat;
import java.util.*;

@Service
public class LocationService {
    private static final Logger logger = LoggerFactory.getLogger(LocationService.class);

    @Autowired
    private LocationDAO locationDAO;

    @Autowired
    private DelayDAO delayDAO;

    @Autowired
    private IpnodeDAO ipnodeDAO;

    @Autowired
    private RecvMessageDAO recvMessageDAO;

    @Autowired
    private StateDAO stateDAO;

    @Autowired
    private SubnetworkDao subnetworkDao;

    private SimpleDateFormat formatter = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");

//    public ArrayList<ArrayList<Location>> getAllLocationByGroup(){
//        ArrayList<Location> res =locationDAO.getAllLocation();
//
//        ArrayList<ArrayList<Location>> voList = new ArrayList<>();
//        ArrayList<Location> temp = new ArrayList<>(res);
//        while (temp.size() != 0) { // 集合数目不等于0，说明至少有一个
//            ArrayList<Location> list = new ArrayList<>();
//            list.add(temp.get(0));
//            ArrayList<Integer> removeIds = new ArrayList<>();
//            removeIds.add(0);
//            for (int i = 0; i < temp.size(); i++) {
//                for (int j = 0; j < list.size(); j++) {
//                    if (!list.contains(temp.get(i)) && distance(list.get(j), temp.get(i)) <= 100) {
//                        list.add(temp.get(i));
//                        removeIds.add(i);
//                        break;
//                    }
//                }
//            }
//            voList.add(list);
//            for (int k = removeIds.size() - 1; k >= 0; k--) {
//                int index = removeIds.get(k);
//                temp.remove(index);
//            }
//        }
//        return voList;
//    }

    // new
    public ArrayList<ArrayList<Location>> getAllLocationByGroup(){
        ArrayList<ArrayList<Location>> ans = new ArrayList<>();

        ArrayList<Location> res =locationDAO.getAllLocation();
        int len = res.size();

        int[] label = new int[len];
        for (int i = 0; i < len; i++) {
            label[i] = 0;
        }

        Queue<Integer> queue = new LinkedList<>();
        while (!isMark(label)) {
            int i = 0;
            for (; i < len; i++) {
                if (label[i] == 0) {
                    break;
                }
            }
            ArrayList<Location> list = new ArrayList<>();
            queue.offer(i);
            label[i] = 1;
            list.add(res.get(i));
            while (!queue.isEmpty()) {
                int index = queue.poll();
                for (int j = 0; j < len; j++) {
                    if (index == j) {
                        continue;
                    } else {
                        double dis = distance(res.get(j), res.get(index));
                        if (dis < 100 && label[j] == 0) {
                            queue.offer(j);
                            label[j] = 1;
                            list.add(res.get(j));
                        }
                    }
                }
            }
            ans.add(list);
        }
//        System.out.println("-------------------子网个数--------------------");
//        System.out.println(ans);
//        System.out.println(ans.size());

        SubNet subNet = new SubNet();
        subNet.setCount(ans.size());
        subNet.setTime(formatter.format(new Date()));
        subnetworkDao.addSub(subNet);
        return ans;
    }

    public boolean isMark(int[] label) {
        for (int i = 0; i < label.length; i++) {
            if (label[i] == 0) {
                return false;
            }
        }
        return true;
    }

    public ArrayList<Location> getAllLocation(){
        return locationDAO.getAllLocation();
    }

    private boolean isSet(List<Location> list, Location location) {
        for (int i = 0; i < list.size(); i++) {
            if (distance(list.get(i), location) <= 100) {
                return true;
            }
        }
        return false;
    }

    //距离计算方法
    public double distance(Location a, Location b) {
        double x1 = a.getX() - b.getX();
        double y1 = a.getY() - b.getY();
        double z1 = a.getZ() - b.getZ();
        return Math.sqrt(x1 * x1 + y1 * y1 + z1 * z1);
    }

    public Location getById(int id) {
        return locationDAO.getById(id);
    }

    public ArrayList<Integer> getAllNodes() {
        return locationDAO.getAllNodes();
    }

    @Transactional(isolation = Isolation.READ_COMMITTED, propagation = Propagation.REQUIRED)
    public int add(Location location) throws InterruptedException {
        ArrayList<Location> locations = locationDAO.getAllLocation();
        for (int i = 0; i < locations.size(); i++) {
            if (locations.get(i).getX() == location.getX() && locations.get(i).getY() == location.getY() &&
                    locations.get(i).getZ() == location.getZ()) {
                return 0;
            }
        }
        State state = new State();
        state.setId(location.getId());
        state.setState(1);

        stateDAO.updateState(state);

        int res =locationDAO.insertLocation(location);

//        // 监听数据库
//        int flag = CommunityUtil.flagMap.get(location.getId());
//        int res = 0;
//        while (true) {
////            Thread.sleep(1000);
//            RecvMessage recvMessage = recvMessageDAO.getById(location.getId());
//
//            if (flag == recvMessage.getFlag()) {
//                System.out.println("flag没有变化");
//            } else {
//                System.out.println("flag变化");
//                CommunityUtil.flagMap.put(location.getId(), recvMessage.getFlag());
//                res = 1;
//                break;
//            }
//        }
        return res;
    }

    public int updateXYZ(int id, int x, int y, int z) {

        ArrayList<Location> locations = locationDAO.getAllLocation();
        for (int i = 0; i < locations.size(); i++) {
            if (locations.get(i).getX() == x && locations.get(i).getY() == y &&
                    locations.get(i).getZ() == z) {
                return 0;
            }
        }

//        int flag = CommunityUtil.flagMap.get(id);
//        int res = 0;
//        while (true) {
//            try {
//                Thread.sleep(1000);
//            } catch (InterruptedException e) {
//                e.printStackTrace();
//            }
//            RecvMessage recvMessage = recvMessageDAO.getById(id);
//            if (flag == recvMessage.getFlag()) {
//                System.out.println("flag没有变化");
//                continue;
//            } else {
//                System.out.println("flag变化");
//                CommunityUtil.flagMap.put(id, recvMessage.getFlag());
//                res = 1;
//                break;
//            }
//        }
        return locationDAO.updateXYZ(id, x, y, z);
    }

    @Transactional(isolation = Isolation.READ_COMMITTED, propagation = Propagation.REQUIRED)
    public Map<String,Object> getData(int id) {
        Map<String, Object> res = new HashMap<>();
        System.out.println("------getData开始------"+ res==null);     //===============================================

        // 获取数据库所有的节点
        ArrayList<Location> list = locationDAO.getAllLocation();

        // 获取xData的数据
        ArrayList<String> xData = new ArrayList<>();
        for (int i = 0; i < list.size(); i++) {
            if (list.get(i).getId() != id) {
                xData.add("N" + list.get(i).getId());
                //xData.add("" + list.get(i).getId());
            }
        }
        res.put("xData", xData);

        // 获取yData的数据
        Ipnode destination = ipnodeDAO.getById(id);  // 获取目的节点
        ArrayList<Double> yData = new ArrayList<>();
        for (int i = 0; i < list.size(); i++) {
            if (list.get(i).getId() != id) {

                // 得到节点的ip
                Ipnode ipnode= ipnodeDAO.getById(list.get(i).getId());

                //WXT 默认初始化
                String sourceip="00.00.00.0";
                try{sourceip = ipnode.getIp();}
                catch (NullPointerException e){
                    System.out.println("ID: "+list.get(i).getId()+" $$$$$$$$$$$$$$$$ipnode==null =>"+(ipnode==null)+" .. "+e);
                }

                ArrayList<Delay> delayList = delayDAO.isExist(sourceip,destination.getIp());
                if (delayList.size() > 0) {
                    double delay = delayDAO.getBySourceip(sourceip, destination.getIp());
                    yData.add(delay);
                } else {
                    yData.add((double) 0);
                }
            }
        }
        res.put("yData", yData);

        ArrayList<Integer> avgData = new ArrayList<>();
        for (int i = 0; i < list.size(); i++) {
            if (list.get(i).getId() != id) {
                // 得到节点的ip
                Ipnode ipnode= ipnodeDAO.getById(list.get(i).getId());

                //WXT  , 设置默认初始值
                String sourceip ="00.00.00.0";
                try{sourceip = ipnode.getIp();}
                catch (NullPointerException e ){
                      System.out.println("LOCALSERVICE : 280 => " +e);
                }

                ArrayList<Delay> delayList = delayDAO.isExist(sourceip,destination.getIp());
                if (delayList.size() > 0) {
                    double delay = delayDAO.getByDelayAVG(sourceip,destination.getIp());
                    avgData.add((int) delay);
                } else {
                    avgData.add(0);
                }
            }
        }
        res.put("avgData", avgData);
        System.out.println("------getData结束------");//=======================================================================
        return res;
    }

    @Transactional(isolation = Isolation.READ_COMMITTED, propagation = Propagation.REQUIRED)
    public Map<String,Object> getPacket() {
        Map<String, Object> res = new HashMap<>();
        System.out.println("------getPacket开始------>>>"+ (res==null));

        // 获取数据库所有的节点
        ArrayList<Location> list = locationDAO.getAllLocation();

        // 获取xData的数据
        ArrayList<String> xData = new ArrayList<>();
        for (int i = 0; i < list.size(); i++) {
            xData.add("N" + list.get(i).getId());
        }
        res.put("xData", xData);

        // 获取yData的数据
        ArrayList<Integer> yData = new ArrayList<>();
        for (int i = 0; i < list.size(); i++) {
            // 得到节点的ip
            Ipnode ipnode= ipnodeDAO.getById(list.get(i).getId());
            String ip;
           // System.out.println("@@@@@@@@@@@@"+ipnode);
            if(ipnode==null || ipnode.getIp() == null ) {System.out.println("!!!!!!!! ipnode==null");
                   ip= "-1";
            }
            else {
                ip=ipnode.getIp();
            }



            Integer count = delayDAO.getPacketCount(ip);
            yData.add(count);
        }
        res.put("yData", yData);
        System.out.println("------getPacket结束------");

        return res;
    }

}

    /*@Transactional(isolation = Isolation.READ_COMMITTED, propagation = Propagation.REQUIRED)
    public Map<String,Object> getData(int id) {
        Map<String, Object> res = new HashMap<>();
//        System.out.println("------getData开始------");

        // 获取数据库所有的节点
        ArrayList<Location> list = locationDAO.getAllLocation();

        // 获取xData的数据
        ArrayList<String> xData = new ArrayList<>();
        for (int i = 0; i < list.size(); i++) {
            if (list.get(i).getId() != id) {
                xData.add("节点" + list.get(i).getId());
            }
        }
        res.put("xData", xData);

        // 获取yData的数据
        Ipnode destination = ipnodeDAO.getById(id);  // 获取目的节点
        ArrayList<Double> yData = new ArrayList<>();
        for (int i = 0; i < list.size(); i++) {
            if (list.get(i).getId() != id) {
                // 得到节点的ip
                Ipnode ipnode= ipnodeDAO.getById(list.get(i).getId());
                String sourceip = ipnode.getIp();
                ArrayList<Delay> delayList = delayDAO.isExist(sourceip,destination.getIp());
                if (delayList.size() > 0) {
                    double delay = delayDAO.getBySourceip(sourceip, destination.getIp());
                    yData.add(delay);
                } else {
                    yData.add((double) 0);
                }
            }
        }
        res.put("yData", yData);

        ArrayList<Integer> avgData = new ArrayList<>();
        for (int i = 0; i < list.size(); i++) {
            if (list.get(i).getId() != id) {
                // 得到节点的ip
                Ipnode ipnode= ipnodeDAO.getById(list.get(i).getId());
                String sourceip = ipnode.getIp();

                ArrayList<Delay> delayList = delayDAO.isExist(sourceip,destination.getIp());
                if (delayList.size() > 0) {
                    double delay = delayDAO.getByDelayAVG(sourceip,destination.getIp());
                    avgData.add((int) delay);
                } else {
                    avgData.add(0);
                }
            }
        }
        res.put("avgData", avgData);

//        System.out.println("------getData结束------");

        return res;
    }*/

/*    @Transactional(isolation = Isolation.READ_COMMITTED, propagation = Propagation.REQUIRED)
    public Map<String,Object> getPacket() {
        Map<String, Object> res = new HashMap<>();
//        System.out.println("------getPacket开始------");

        // 获取数据库所有的节点
        ArrayList<Location> list = locationDAO.getAllLocation();

        // 获取xData的数据
        ArrayList<String> xData = new ArrayList<>();
        for (int i = 0; i < list.size(); i++) {
            xData.add("节点" + list.get(i).getId());
        }
        res.put("xData", xData);

        // 获取yData的数据
        ArrayList<Integer> yData = new ArrayList<>();
        for (int i = 0; i < list.size(); i++) {
            // 得到节点的ip
            Ipnode ipnode= ipnodeDAO.getById(list.get(i).getId());
            String ip = ipnode.getIp();
            Integer count = delayDAO.getPacketCount(ip);
            yData.add(count);
        }
        res.put("yData", yData);
//        System.out.println("------getPacket结束------");

        return res;*/
