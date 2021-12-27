
var orderGroups = [];

function initOrders() {

    try{
        orderGroups.push({"insName":"LLS","orderGroupDate":"2018-05-23","subOrders":[]});
        orderGroups.push({"insName":"LLG","orderGroupDate":"2018-05-24","subOrders":[]});

        for(var index=0; index < 10; index ++){
            orderGroups[0].subOrders.push( { orderId:"LLS" + index,  stateText:"Executed", timeText:"12:00:0" +index , lotText:"1."+index, typeText:"DQ", priceText:"12345"});
            orderGroups[1].subOrders.push( { orderId:"LLG" + index, stateText:"Placed", timeText:"10:20:3" +index, lotText:"2."+index, typeText:"Limit", priceText:"5678.90"});
        }
    }
    catch(ex){
        console.log("initOrders() error:"  + ex);
    }
}
