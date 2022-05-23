 $(function(){
    $("#publishBtn").click(publish);
    $("#initBtn").click(initNode);
    $("#updateLocationBtn").click(updateLocation);
    $("#deleteIdBtn").click(deleteIdBtn);


});

function publish() {
    $("#publishModal").modal("hide");

    //获取内容
    var id = $("#node-id").val();
  //  var pilot = $("#node-pilot").val();
   // var ammunition = $("#node-ammunition").val();
  //  var totalAmmunition = $("#node-totalAmmunition").val();
   // var oil = $("#node-oil").val();
 //   var totalOil = $("#node-totalOil").val();
    var x = $("#node-location-x").val();
    var y = $("#node-location-y").val();
    var z = $("#node-location-z").val();
    //发送异步请求
    $.post(
        CONTEXT_PATH + "/add",
        {
            "id": id,
            //"pilot": pilot,
          //  "ammunition": ammunition,
           // "totalAmmunition": totalAmmunition,
           // "oil": oil,
          //  "totalOil": totalOil,
            "x": x,
            "y": y,
            "z": z,
        },
        function (data) {
            data = $.parseJSON(data);
            //在提示框中显示返回消息
            $("#hintBody").text(data.msg);
            //显示提示框
            $("#hintModal").modal("show");
            //两秒后自动隐藏提示框
            setTimeout(function () {
                $("#hintModal").modal("hide");
                if (data.code == 0) {
                    window.location.reload();
                }
            }, 2000);
        }
    );
}


function initNode() {

    $("#initModal").modal("hide");

    var nodeid = $("#nodeid").val();
    var bandwidth = $("#bandwidth").val();
    // var nodeinit = $("#nodeinit").is(":checked");
    console.log(nodeid);
    console.log(bandwidth);
    // console.log(nodeinit);

    $.post(
        CONTEXT_PATH + "/init/",
        {
            "id": nodeid,
            "bandwidth": bandwidth,
            "nodeinit": false
        },
        function(data) {
            data = $.parseJSON(data);
            if(data.code == 0) {

                //在提示框中显示返回消息
                $("#hideBody").text(data.msg);
                //显示提示框
                $("#hideModal").modal("show");
                //两秒后自动隐藏提示框
                setTimeout(function () {
                    $("#hideModal").modal("hide");
                    //刷新页面
                    window.location.reload();
                    // if (data.code == 0) {
                    //     window.location.reload();
                    // }
                }, 2000);
            } else {
                // alert(data.msg);
                // 在提示框中显示返回消息
                $("#hideBody").text(data.msg);
                //显示提示框
                $("#hideModal").modal("show");
                //两秒后自动隐藏提示框
                setTimeout(function () {
                    $("#hideModal").modal("hide");
                    window.location.reload();
                }, 2000);
            }
        }
    );
}

function updateLocation() {
    $("#updateLocationModal").modal("hide");

    // 发送AJAX请求之前,将CSRF令牌设置到请求的消息头中.
    // var token = $("meta[name='_csrf']").attr("content");
    // var header = $("meta[name='_csrf_header']").attr("content");
    // $(document).ajaxSend(function(e, xhr, options){
    //     xhr.setRequestHeader(header, token);
    // });

    //获取内容
    var id = $("#node-update-id").val();
    var x = $("#node-location2-x").val();
    var y = $("#node-location2-y").val();
    var z = $("#node-location2-z").val();
    //发送异步请求
    $.post(
        CONTEXT_PATH + "/updateXYZ",
        {
            "id": id,
            "x": x,
            "y": y,
            "z": z,
        },
        function (data) {
            data = $.parseJSON(data);
            //在提示框中显示返回消息
            $("#xyzBody").text(data.msg);
            //显示提示框
            $("#xyzModal").modal("show");
            //两秒后自动隐藏提示框
            setTimeout(function () {
                $("#xyzModal").modal("hide");
            }, 2000);
        }
    );
}


function deleteIdBtn() {
    $("#deleteIdModal").modal("hide");

    //获取内容
    var id = $("#delete-id").val();
    console.log(id);
    //发送异步请求
    $.post(
        CONTEXT_PATH + "/del",
        {
            "id": id
        },
        function (data) {
            data = $.parseJSON(data);
            //在提示框中显示返回消息
            $("#xyBody").text(data.msg);
            //显示提示框
            $("#xyModal").modal("show");
            //两秒后自动隐藏提示框
            setTimeout(function () {
                $("#xyModal").modal("hide");
                window.location.reload();
            }, 2000);
        }
    );
}
