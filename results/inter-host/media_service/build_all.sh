#!/bin/bash

ssh -p 22 lgw@192.168.1.129 "cmake --build /tmp/SPheromone129 \
--target manager coordinator scheduler executor \
exp02_store_review_4 exp02_upload_text_3 exp02_upload_uniqueid_3 exp02_upload_rating_3 exp02_upload_movieid_3 \
exp02_upload_rating_2 exp02_upload_uniqueid_1 exp02_upload_text_1 exp02_upload_movieid_1 exp02_register_movieid_1 \
exp02_web_server_0 exp02_register_test1 exp02_invoker_test1 -- -j 16"

ssh -p 22 lgw@192.168.1.126 "cmake --build /tmp/SPheromone126 \
--target manager coordinator scheduler executor \
exp02_store_review_4 exp02_upload_text_3 exp02_upload_uniqueid_3 exp02_upload_rating_3 exp02_upload_movieid_3 \
exp02_upload_rating_2 exp02_upload_uniqueid_1 exp02_upload_text_1 exp02_upload_movieid_1 exp02_register_movieid_1 \
exp02_web_server_0 exp02_register_test1 exp02_invoker_test1 -- -j 16"
