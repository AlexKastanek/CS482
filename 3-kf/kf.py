# kf.py updated file for robot estimation assignment
# pset[3]
# (C) 2017 David Feil-Seifer


import numpy as np
from numpy.linalg import inv
import math
from scipy import stats
import scipy.stats

# kf_update: update state estimate [u, sigma] with new control [xdot] and measurement [z]
# 	parameters:
#			u : 2x1 vector with state estimate (x) at time t-1 and control (xdot) at time t
#			sigma: 2x2 matrix with covariance at time t-1
#			z (int): observed (uncertain) measurement of state (x) at time t
#	returns: [u sigma] updated state with estimate at time t

def kf_update(u, sigma, z):

	dt = 1.0

	#predictive step
	f = np.mat([[1, dt], [0, 1]])
	#h = np.mat([[1,0],[1,0]])
	h = np.matrix([1,0])
	#q = np.mat([[0,0],[0,50]])
	#r = np.mat([[20,0],[0,20]])
	g = np.matrix([[dt * dt], [dt/2]])
	q = g * np.power(g.T, 5 * 5)
	r = 7 * 7
	sigma = (f * sigma * f.T) + q

	#corrective step
	s = (h * sigma * h.T) + r
	k = sigma * h.T * inv(s)
	u = f*u + k*(z - (h*f*u))
	sigma = (np.identity(2) - (k*h))*sigma

	return [u, sigma]

false_readings = np.ones(10)
true_readings = np.ones(10)
false_ests = np.full(10, 0.5)
true_ests = np.full(10, 0.5)

# door_update: update estimate of door locations
# 	parameters:
#			u : 2x1 vector with state estimate (x) at time t-1 and control (xdot) at time t-1
#			sigma: 2x2 matrix with covariance at time t-1
#			d (binary): door sensor at time t-1 
#			door_dist (array of size 10): probability (0..1) that a door exists at each location (0..9)
#	returns: [door_dist] updated door distribution

def door_update(u, sigma, d, door_dist):

	# determine section
	section = u[0] % 1000 / 100
	section = int(section)
	print("Current section: %d" %(section))

	# increment readings
	if d:
		true_readings[section] += 1
	else:
		false_readings[section] += 1
	#print("true_readings: %s" % (true_readings))
	#print("false_readings: %s" % (false_readings))

	# calculate P(d), probability there is a door
	pd = true_readings[section] / (true_readings[section] + false_readings[section])
	# and P(~d), probability there is no door
	'''
	pnotd = 1 - pd
	'''

	# initialize P(s), probability sensor detects door
	ps = 0.4
	# and P(~s), probability sensor does not detect a door
	'''
	pnots = 1 - ps
	'''

	# initialize P(s | d), probability sensor detects door given there is a door
	pSGivenD = 0.6
	'''
	# initialize P(s | ~d), probability sensor detects door given there is no door
	pSGivenNotD = 0.2
	# initialize P(~s | d), probability sensor does not detect door given there is a door
	pNotSGivenD = 0.4
	# and P(~s | ~d), probability sensor does not detect door given there is no door
	pNotSGivenNotD = 0.8
	'''

	## Guess I was overcomplicating this a bit
	# calculate P(d | s), probability there is a door given sensor detects door
	'''
	if d:
		print(pd)
		#door_dist[section] = float((pSGivenD * pd) / ps)# * float(((pSGivenNotD * pnotd) / ps))
		true_ests[section] += float((pSGivenD * pd) / ps)
		print("sensor detected door")
		print(float((pSGivenD * pd) / ps)) # prob door exists given sensor detects door
		print(float((pSGivenNotD * pnotd) / ps)) # prob door does not exist given sensor detects door
	# or P(d | ~s), probability there is no door given sensor does not detect door
	else:
		print(pd)
		pDGivenNotS = float((pNotSGivenD * pd) / pnots)
		pNotDGivenNotS = float((pNotSGivenNotD * pnotd) / pnots)
		#door_dist[section] = float((pNotSGivenD * pd) / pnots)# * float(((pNotSGivenNotD * pnotd) / pnots))
		false_ests[section] += float((pNotSGivenD * pd) / pnots)
		print("sensor detected no door")
		print(pDGivenNotS) # prob door exists given sensor detects no door
		print(pNotDGivenNotS) # prob door does not exist given sensor detects no door
	'''
	door_dist[section] = float((pSGivenD * pd) / ps)

	return door_dist


















