#include <mutex>

#include "../includes/baker.h"
#include "../includes/externs.h"
#include "../includes/constants.h"
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
	while (donutCount != 0) {

		if (b.addDonut(d) == false){
			anOrder.boxes.push_back(b);
			b.clear();
		}
		donutCount--;
	}



	order_out_Vector.push_back(anOrder);


}

void Baker::beBaker() {
	
	while (true){
		unique_lock<mutex> lck(mutex_order_inQ);
		while(order_in_Q.size() == 0 && b_WaiterIsFinished == false){
			cv_order_inQ.wait(lck);
		}

		if (order_in_Q.size() > 0){
				bake_and_box(order_in_Q.front());
				order_in_Q.pop();

				lck.unlock();
				cv_order_inQ.notify_all();
		}
		if (b_WaiterIsFinished){
			while (order_in_Q.size() != 0){
				bake_and_box(order_in_Q.front());
				order_in_Q.pop();

			}
			break;
		}



	}

}
