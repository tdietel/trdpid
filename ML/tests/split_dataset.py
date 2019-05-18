from datatools.dataset_funcs import *

tracks, info_set = load_whole_named_dataset('all_tracks_with_4_or_more_tracklets')

e_tracks = tracks[info_set[:,0]==1]
p_tracks = tracks[info_set[:,0]==0]
e_info_set = info_set[info_set[:,0]==1]
p_info_set = info_set[info_set[:,0]==0]

e_train_tracks = e_tracks[:1000]
p_train_tracks = p_tracks[:1000]
e_train_info_set = e_info_set[:1000]
p_train_info_set = p_info_set[:1000]
train_tracks = np.concatenate([e_train_tracks, p_train_tracks])
train_info_set = np.concatenate([e_train_info_set, p_train_info_set])
randomize = np.arange(len(train_tracks))
np.random.shuffle(randomize)
train_tracks = train_tracks[randomize]
train_info_set = train_info_set[randomize]
save_dataset('small_train', train_tracks, train_info_set, -1)

e_test_tracks = e_tracks[:1000]
p_test_tracks = p_tracks[:1000]
e_test_info_set = e_info_set[:1000]
p_test_info_set = p_info_set[:1000]
test_tracks = np.concatenate([e_test_tracks, p_test_tracks])
test_info_set = np.concatenate([e_test_info_set, p_test_info_set])
randomize = np.arange(len(test_tracks))
np.random.shuffle(randomize)
test_tracks = test_tracks[randomize]
test_info_set = test_info_set[randomize]
save_dataset('small_test', test_tracks, test_info_set, -1)