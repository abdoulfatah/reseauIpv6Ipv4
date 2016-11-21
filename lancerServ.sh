#!/bin/bash

VBoxManage setproperty machinefolder "/tmp"

cd ../vm1
vagrant up
cd ../vm3
vagrant up
cd ../vm16
vagrant up
cd ../vm26
vagrant up
cd ../vm36
vagrant up
