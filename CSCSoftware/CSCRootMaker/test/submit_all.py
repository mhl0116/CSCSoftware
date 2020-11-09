import sys, os
import time
import itertools
import numpy
import json

from metis.Sample import DBSSample, DirectorySample, Sample
from metis.CondorTask import CondorTask
from metis.StatsParser import StatsParser

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--tag", help = "tag to identify this set of babies", type=str)
parser.add_argument("--filter", help = "only process mc/data with some requirement (e.g. 2016MC, 2017Data)", default="", type=str)
parser.add_argument("--soft_rerun", help = "don't remake tarball", action="store_true")
parser.add_argument("--skip_local", help = "don't submit jobs for local samples", action = "store_true")
parser.add_argument("--skip_central", help = "don't submit jobs for central samples", action = "store_true")
args = parser.parse_args()

# for central inputs
#dsdefs = []
## datasetname, filesPerOutput, filtername
from dsdefs_centralds import dsdefs
# for local inputs
local_sets = []

if not args.skip_local:
    local_sets = [
        ("HHggtautau_Era2018_private", "/hadoop/cms/store/user/hmei/miniaod_runII/HHggtautau_2018_20201002_v1_STEP4_v1/", 200, "")
    ]

# some job configurations
job_dir = "rawreco_runII/zerobias/"
job_tag = args.tag
job_filter = args.filter
hadoop_path = "{0}".format(job_dir)

cmssw_ver = "CMSSW_10_6_8"

DOSKIM = False 

#exec_path = "condor_exe_%s.sh" % args.tag
exec_path = "condor_exe.sh"
#tar_path = "nanoAOD_package_%s.tar.gz" % args.tag

if not args.soft_rerun:
    os.system("rm -rf tasks/*" + args.tag + "*")
    os.system("rm package.tar.gz")
    os.system("XZ_OPT='-3e -T24' tar -Jc --exclude='.git' --exclude='*.root' --exclude='*.tar*' --exclude='*.out' --exclude='*.err' --exclude='*.log' --exclude '*.nfs*' -f package.tar.gz ../../%s" % cmssw_ver)

    #os.system("cp package.tar.gz /hadoop/cms/store/user/smay/FCNC/tarballs/%s" % tar_path)
    #os.system("hadoop fs -setrep -R 30 /cms/store/user/smay/FCNC/tarballs/%s" % tar_path)

total_summary = {}
while True:
    allcomplete = True

    # Loop through central samples
    for ds,fpo,args in dsdefs[:]:
        if (job_filter != "") and (args not in job_filter) : continue         
        sample = DBSSample( dataset=ds )
        print(ds, args)

        task = CondorTask(
                sample = sample,
                open_dataset = False,
                files_per_output = fpo,
                output_name = "test_rawreco.root",
                tag = job_tag,
        	cmssw_version = cmssw_ver,
                executable = exec_path,
                tarfile = "./package.tar.gz",
                condor_submit_params = {"sites" : "T2_US_UCSD", "container":"/cvmfs/singularity.opensciencegrid.org/cmssw/cms:rhel7-m20201010"},
                #condor_submit_params = {"sites" : "T2_US_UCSD,T2_US_CALTECH,T2_US_MIT,T2_US_WISCONSIN,T2_US_Nebraska,T2_US_Purdue,T2_US_Vanderbilt,T2_US_Florida"},
                special_dir = hadoop_path,
                arguments = args.replace(" ","|")
                )
        task.process()
        allcomplete = allcomplete and task.complete()
        # save some information for the dashboard
        total_summary[ds] = task.get_task_summary()
        with open("summary.json", "w") as f_out:
            json.dump(total_summary, f_out, indent=4, sort_keys=True)


    # Loop through local samples
    #for ds,loc,fpo,args in local_sets[:]:
    #    sample = DirectorySample( dataset = ds, location = loc )
    #    files = [f.name for f in sample.get_files()]
    #    print "For sample %s in directory %s, there are %d input files" % (ds, loc, len(files))
    #    #for file in files:
    #    #    print file

    #    task = CondorTask(
    #            sample = sample,
    #            open_dataset = True,
    #            files_per_output = fpo,
    #            output_name = "test_nanoaod.root",
    #            tag = job_tag,
    #            cmssw_version = cmssw_ver,
    #            executable = exec_path,
    #            tarfile = "./package.tar.gz",
    #            condor_submit_params = {"sites" : "T2_US_UCSD"},
    #            special_dir = hadoop_path,
    #            arguments = args.replace(" ","|")
    #    )
    #    task.process()
    #    allcomplete = allcomplete and task.complete()
    #    # save some information for the dashboard
    #    total_summary[ds] = task.get_task_summary()
    #    with open("summary.json", "w") as f_out:
    #        json.dump(total_summary, f_out, indent=4, sort_keys=True)


    # parse the total summary and write out the dashboard
    StatsParser(data=total_summary, webdir="~/public_html/dump/metis_rawreco/").do()
    os.system("chmod -R 755 ~/public_html/dump/metis_rawreco")
    if allcomplete:
        print ""
        print "Job={} finished".format(job_tag)
        print ""
        break
    print "Sleeping 1000 seconds ..."
    time.sleep(1000)
