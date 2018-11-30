load('PLSsampledata.mat')
PLSsampledata_vec = vec2mat(PLSsampledata,8);
save('PLSsampledata_vec.mat','PLSsampledata_vec');