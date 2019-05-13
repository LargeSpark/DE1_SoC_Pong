	static unsigned int lookup[91] = { 0, 1745, 3490, 5234, 6976, 8716, 10453, 12187, 13917,
						15643, 17365, 19081, 20791, 22495, 24192, 25882, 27564,
						... etc. ...
	if (degree <= 90){
		return (float) lookup[degree]/100000;
	}
	else if ((degree > 90) && (degree <= 180)){
		return (float) lookup[180-degree]/100000;
	}
	else if ((degree > 180) && (degree <= 270)){
		return (float) -lookup[degree-180]/100000;
	}
	else if ((degree > 270) && (degree <= 360)){
		return (float) -lookup[360-degree]/100000;
	} else return 0;
}