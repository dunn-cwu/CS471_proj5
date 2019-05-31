
require("tidyverse")
library(ggplot2)

# My input files are in three different folders:
#
# fss/
# fsb/
# fsnw/
# 
# And the files are named [DATASET NUMBER]-gantt.csv
# These files have to be in a specific format, see example
# input files for format.
#
# I specify the folder with the alg variable below
#
# algName variable below is used in the title
#
# dataset is the integer data set being loaded, which will be
# appended to the file name above.
#
# numM = number of machines in the data set
#
# numJ = number of jobs in the data set.
#
# Make sure you update the path below so that it points to the correct input files on your machine

alg = "fsnw"
algName = "Flow Shop With No Wait"
dataset = "1"
numM = 5
numJ = 20

inputFile <- sprintf("~/Desktop/cs471/proj5/results/gantt/%s/%s-gantt.csv", alg, dataset)
title <- sprintf("%s - Data Set: %s", algName, dataset)

acts = c()
els = c()

for (i in 1:numM)
{
  acts[[i]] <- paste("Machine", i)
}

for (i in 1:numJ)
{
  els[[i]] <- paste("Job", i)
}

gantt <- read.csv(inputFile, h=T)

g.gantt <- gather(gantt, "state", "time", 4:5) %>% mutate(time, Machine=factor(Machine, acts[length(acts):1]), Job=factor(Job, els))

# If you want to show the legend, switch the comments out on the to lines below beginning with 'theme'
# In other words, comment out the first line with 'theme' and then un-comment the second
ggplot(g.gantt, aes(time, Machine, color = Job, group=Item)) +
  theme(legend.position="none") +
  # theme(legend.direction ="horizontal",legend.position = "bottom") +
  scale_x_continuous(breaks = round(seq(0, 2000, by = 10),1)) + # 0-2000 is the range of the X axis ticks, by = 100 is the step size
  geom_line(size = 30) + # CHANGE THIS VALUE TO CHANGE HEIGHT OF GRAPH BARS
  labs(x="Time Units", y=NULL, title=title)
  
