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
hist_data = [dframe['NoReserveTime'],dframe['ReserveAllTime'],
             dframe['ReserverHalfTime'],dframe['ReserveQuarterTime']]
group_labels = ['Reserve_0%','Reserve_100%','Reserve_50%','Reserve_25%']
fig = ff.create_distplot(hist_data,group_labels,show_hist=False,show_rug=False)
div1 = plotly.offline.plot(fig, include_plotlyjs=False, output_type='div')
print(div1)