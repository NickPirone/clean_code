import numpy as np
import pandas as pd
import plotly
import plotly.figure_factory as ff
import sys
import pprint

def normalize_series(dframe, series_col_index) :
	y = dframe[series_col_index]
	log_y = np.log(y)
	diff_log_y = log_y - np.average(log_y)
	var_log_y = sum( diff_log_y ** 2 ) / len(y)
	stdev_log_y = np.sqrt(var_log_y)
	cnt = 0
	for i in range(0, len(log_y)) :
		if( abs(diff_log_y[i]) > (4*stdev_log_y) ):
			cnt += 1
			if(i > 5): 
				y.iloc[i] = y.iloc[i-1]
			else:
				y.iloc[i] = y.iloc[i+1]
	#print('cleaned:  ', cnt)

dframe = pd.read_csv(sys.argv[1]) #index_col=0
hist_data = [dframe['1thread'],dframe['2thread'],dframe['3thread'],dframe['4thread'],
             dframe['5thread'],dframe['6thread'],dframe['7thread'],dframe['8thread']]
group_labels = ['Single_Threaded','2_Threads','3_Threads','4_Threads',
            '5_Threads','6_Threads','7_Threads','8_Threads']
fig = ff.create_distplot(hist_data,group_labels,show_hist=False)
div1 = plotly.offline.plot(fig, include_plotlyjs=False, output_type='div')
print(div1)