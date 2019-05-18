import numpy as np
import os
import sys

# Usage:
# python3 generate_numpy_track_datafiles /path/to/outermost/data/folder /path/to/output/folder/ num_tracks_per_file [max_mod_nsigma_e] [max_mod_nsigma_p]
#
# will scan /path/to/outermost/data/folder and all subdirectories for files named pythonDict.txt and loops through each track and appends to a numpy
# array until a length of num_tracks_per_file is reached. Then the array is saved to /path/to/output/folder/ with an increasing index. This process continues until
# all python dicts have been read. Upon termination, if the final numpy array is not full, its contents are discarded.
# 3 arrays are generated per set of tracklets: i_tracks.npy, i_labels.npy, i_num_tracklets.npy

path = sys.argv[1]
output_folder = sys.argv[2]
tracks_per_file = int(sys.argv[3])
max_mod_nsigma_e = sys.argv[4] if len(sys.argv) >= 5 else None
max_mod_nsigma_p = sys.argv[5] if len(sys.argv) >= 6 else None

files = []
for r, d, f in os.walk(path):
    for file in f:
        if 'pythonDict.txt' in file:
            files.append(os.path.join(r, file))

tracklet_data_set = np.zeros((tracks_per_file, 6, 17, 24), dtype='float32')
label_data_set = np.zeros((tracks_per_file))
tracklets_in_track_data_set = np.zeros((tracks_per_file))
track_count = 0

for i, fil in enumerate(files):
    print('%d / %d' % (i, len(files)), fil)
    f = open(fil)
    r = f.read()
    try:
        exec('dic = ' + r + '}')
        for track in dic.values():
            export=False

            num_tracklets = np.sum(['layer' in key for key in track.keys()])

            nsigmae = track['nSigmaElectron']
            nsigmap = track['nSigmaPion']

            if abs(track['pdgCode']) == 11:
                label = 1
                if max_mod_nsigma_e is None or np.abs(nsigmae) <= max_mod_nsigma_e:
                    export = True
            elif abs(track['pdgCode']) == 211:
                label = 0
                if max_mod_nsigma_p is None or np.abs(nsigmap) <= max_mod_nsigma_p:
                    export = True
            else:
                print('UNRECOGNIZED pdgCode %d' % track['pdgCode'])

            if export:
                for track_no in range(num_tracklets):
                    arr = np.asarray(track['layer %d' % track_no], dtype='float32')

                    assert arr.shape == (17, 24)

                    tracklet_data_set[track_count % tracks_per_file][track_no] = arr
                    label_data_set[track_count % tracks_per_file] = label
                    tracklets_in_track_data_set[track_count % tracks_per_file] = num_tracklets

                track_count += 1

                if track_count % tracks_per_file == 0:
                    np.save(output_folder + "%d_tracks.npy" % ((track_count // tracks_per_file)-1), tracklet_data_set)
                    np.save(output_folder + "%d_labels.npy" % ((track_count // tracks_per_file)-1), label_data_set)
                    np.save(output_folder + "%d_num_tracklets.npy" % ((track_count // tracks_per_file)-1), tracklets_in_track_data_set)

    except Exception as e:
        print(e)