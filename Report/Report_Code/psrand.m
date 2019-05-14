for i = 0:100
angle = round(rand()*360);
while(angle > 80 && angle < 100) || (angle > 260 && angle < 270)
angle = round(rand()*360);
end
fprintf('%d, ', angle);
end