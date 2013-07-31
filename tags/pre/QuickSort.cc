void QuickSort(Float_t number[], int left, int right)
{
#define SWAP(x,y) {int t; t = x; x = y; y = t;} 
	int max=200;
	if(left < right)
	{ 
		int i = left; 
		int j = right + 1; 
		while(1)
		{ 
			// 向右找
            while(i + 1 <  max&& number[++i] < number[left]) ;  
            // 向左找  
            while(j -1 > -1 && number[--j] > number[left]) ;  
            if(i >= j) 
                break; 
            SWAP(number[i], number[j]); 
        } 
        SWAP(number[left], number[j]); 
        QuickSort(number, left, j-1);   // 對左邊進行遞迴 
        QuickSort(number, j+1, right);  // 對右邊進行遞迴 
   	} 
} 
