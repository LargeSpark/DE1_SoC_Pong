file_handle = fopen('sinvals105.txt','w');

err_max = 0;

for theta = 0:89
    fprintf(file_handle, '%d, ', round(sind(theta)*10^5));
    if (abs((sind(0:90)*10^5-(round(sind(0:90)*10^5)))/10^5) > err_max)
        err_max = (sind(0:90)*10^5-(round(sind(0:90)*10^5)))/10^5;
    end
end

fprintf(file_handle, '%d', round(sind(90)*10^5));
fprintf(file_handle, '\n\n Max error = %g', err_max);

fclose(file_handle);

plot(sind(0:90), 'b');
hold on;
plot((round(sind(0:90)*10^5))/10^5, 'rx');
title('Difference, sin and fastsin');
