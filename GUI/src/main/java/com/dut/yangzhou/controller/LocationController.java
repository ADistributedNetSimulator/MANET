package com.dut.yangzhou.controller;

import com.dut.yangzhou.dao.RecvMessageDAO;
import com.dut.yangzhou.model.Location;
import com.dut.yangzhou.model.PoorChannel;
import com.dut.yangzhou.model.RecvMessage;
import com.dut.yangzhou.service.LocationService;
import com.dut.yangzhou.service.PoorChannelService;
import com.dut.yangzhou.util.CommunityUtil;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;

import javax.jws.WebParam;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;


@Controller
public class LocationController {

    private static Logger logger = LoggerFactory.getLogger(LocationController.class);

    @Autowired
    private LocationService locationService;

    @Autowired
    private PoorChannelService poorChannelService;
    @Autowired
    private RecvMessageDAO recvMessageDAO;

    @RequestMapping(path = "/", method = RequestMethod.GET)
    public String root() {
//        System.out.println("---root---");
        return "forward:/index";
    }

    @RequestMapping(path = "/index")
    public String  getpage(Model model) {
//        System.out.println("---index---");
        ArrayList<Location> list = locationService.getAllLocation();

//        System.out.println("===="+list);
        List<Map<String, Object>> voList = new ArrayList<>();
        if (list.size() != 0) {
//            System.out.println("=======");
            for (Location location : list) {

                Map<String, Object> map = new HashMap<>();
                //WXT:=-------------------------------------------------------
                if(location.equals(null)|| location.getId()==0){
                    System.out.println("location.equals(null)|| location.getId()==0");
                    //System.out.println("WARNING !!WXT");
                }

                map.put("location", location);
                //end-----------------------------------------------------------------------

                voList.add(new HashMap<String, Object>(map));
            }
            model.addAttribute("voList", voList);
        }
        return "/index";
    }


    @RequestMapping(path = {"/location"})
    @ResponseBody
    public ArrayList<ArrayList<Location>> location(){
//        System.out.print("--location--");
        ArrayList<ArrayList<Location>> vo = locationService.getAllLocationByGroup();
//        System.out.println(vo);
        Map<String, Object> map = new HashMap<>();
        map.put("vo", vo);
//        return CommunityUtil.getJSONString(0, "OK", map);
        return vo;
    }

    @RequestMapping(path = "/add", method = RequestMethod.POST)
    @ResponseBody
    public String addNode(int id,
                          int x, int y, int z) throws InterruptedException {
        Location location = new Location();
        location.setId(id);
        location.setX(x);
        location.setY(y);
        location.setZ(z);

//        System.out.println(location);


        int res = locationService.add(location);
//        if (res == 1) {
//            RecvMessage message = recvMessageDAO.getById(id);
//            return CommunityUtil.getJSONString(0, "发送成功" + message.getRecvMessage());
//        } else {
//            return CommunityUtil.getJSONString(0, "服务端错误");
//        }

        return CommunityUtil.getJSONString(0, "Success");
    }

    @RequestMapping(path = "/updateXYZ", method = RequestMethod.POST)
    @ResponseBody
    public String updateXYZ(int id, int x, int y, int z) {

        int num = locationService.updateXYZ(id, x, y, z);

        if (num != 0) {
            return CommunityUtil.getJSONString(0, "Update successfully");
        } else {
            return CommunityUtil.getJSONString(0, "Update failed");
        }
    }

    @RequestMapping(path = "/info/{id}", method = RequestMethod.GET)
    @ResponseBody
    public String location(@PathVariable("id") int id, Model model) {

        Map<String, Object> map = new HashMap<>();

        Location location = locationService.getById(id);

        map.put("location", location);
        return CommunityUtil.getJSONString(0, "Success", map);
    }
    @RequestMapping(path = "/test/{id}", method = RequestMethod.GET)
    @ResponseBody
    public String testId(@PathVariable("id") int id, Model model) {
        Location location = locationService.getById(id);
        if (location != null) {
            return CommunityUtil.getJSONString(1, "The node already exits!");
        } else {
            return CommunityUtil.getJSONString(0, "The node doesn't exit!");
        }
    }

    @RequestMapping(path = "/test2/{id}", method = RequestMethod.GET)
    @ResponseBody
    public String testId2(@PathVariable("id") int id, Model model) {
        Location location = locationService.getById(id);
        if (location != null) {
            return CommunityUtil.getJSONString(1, "The node already exits!");
        } else {
            return CommunityUtil.getJSONString(0, "The node doesn't exit!");
        }
    }

    @RequestMapping(path = {"/graph"})
    public String graph(Model model){
//        System.out.println("-----");
        ArrayList<Location> res =locationService.getAllLocation();
//        System.out.println(res);
        model.addAttribute("title", "3D graph of node location");
        return "location";
    }

    @RequestMapping(path = {"/test"})
    public String test(Model model){
//        System.out.println("-----");
        ArrayList<Location> res =locationService.getAllLocation();
//        System.out.println(res);
//        model.addAttribute("title", "3D graph of node location");
        return "test";
    }

    @RequestMapping(path = {"/columnar"})
    public String columnar(Model model){
        return "columnar";
    }

    @RequestMapping(path = {"/getData/{id}"})
    @ResponseBody
    public Map<String,Object> getData(@PathVariable("id") int id){
//        System.out.println("--getData--");
//        System.out.println(id);
        Map<String, Object> res = locationService.getData(id);
        return res;
    }

    @RequestMapping(path = {"/getPacket"})
    @ResponseBody
    public Map<String,Object> getPacket(){
        Map<String, Object> res = locationService.getPacket();
        return res;
    }
    @RequestMapping(path = {"/poorupdate"})
    @ResponseBody
    public void poorupdate(int id,Model model){
        PoorChannel poorChannel=new PoorChannel(id);
        int ans=poorChannelService.UpdatePoorChanel(poorChannel);
        model.addAttribute("poorC",ans);
    }

}
