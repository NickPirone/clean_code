import numpy as np
import pandas as pd
import plotly
import plotly.figure_factory as ff
import sys
import pprint



dframe_hash = pd.read_csv(sys.argv[1]) #index_col=0
dframe_tree = pd.read_csv(sys.argv[2])

#constructor timing
hist_data = [dframe_hash['Constructor'],dframe_tree['Constructor']]
group_labels = ['unordered_map','map']
fig = ff.create_distplot(hist_data,group_labels,show_hist=False,show_rug=False)
fig['layout'].update(title='Constructor')
div1 = plotly.offline.plot(fig, include_plotlyjs=False, output_type='div')
print(div1)

#Access timing
for i in [10,20,40]:
    column_str = 'Access' + str(i);
    hist_data = [dframe_hash[column_str], dframe_tree[column_str]]
    group_labels = ['unordered_map','map']
    fig = ff.create_distplot(hist_data,group_labels,show_hist=False,show_rug=False)
    fig['layout'].update(title=column_str)
    div1 = plotly.offline.plot(fig, include_plotlyjs=False, output_type='div')
    print(div1)

#Insertion timing
for i in [10,160,640]:
    column_str = "Insert" + str(i);
    hist_data = [dframe_hash[column_str], dframe_tree[column_str]]
    group_labels = ['unordered_map','map']
    fig = ff.create_distplot(hist_data,group_labels,show_hist=False,show_rug=False)
    fig['layout'].update(title=column_str)
    div1 = plotly.offline.plot(fig, include_plotlyjs=False, output_type='div')
    print(div1)