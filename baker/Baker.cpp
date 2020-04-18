#include <mutex>

#include "../includes/baker.h"
#include "../includes/externs.h"
using namespace std;

Baker::Baker(int id):id(id)
{
}

Baker::~Baker()
{
}

void Baker::bake_and_box(ORDER &anOrder) {
	int donutCount = anOrder.number_donuts;
	
	Box b;
	DONUT d;

	if (anOrder.number_donuts < 13){
		while (donutCount != 0){
			b.addDonut(d);
			donutCount--;
		}
		anOrder.boxes.push_back(b);
		b.clear();
	}
	while(donutCount != 0) {
		if(!b.addDonut(d)) {
			anOrder.boxes.push_back(b);
			b.clear();
		}
		else {
			donutCount--;
		}
	}
	if (b.size() > 0){
		anOrder.boxes.push_back(b);
	}
}

void Baker::beBaker() {
	bool doFirst = true;
	while(!b_WaiterIsFinished || !order_in_Q.empty() || doFirst){
		doFirst = false;
		unique_lock<mutex> lck(mutex_order_inQ);
		while(order_in_Q.size() == 0 && !b_WaiterIsFinished) {
			cv_order_inQ.wait(lck);
		}

		ORDER order;
		if(!order_in_Q.empty()) {
			order = order_in_Q.front();
			order_in_Q.pop();
			bake_and_box(order);
			lock_guard<mutex> sending_ticket(mutex_order_outQ);
			order_out_Vector.push_back(order);
		}
	}
}
