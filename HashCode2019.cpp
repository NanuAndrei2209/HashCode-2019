#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#define RAZA 200

using namespace std;

class Photo {
public:
    int id;
    bool isHorizontal;
    int nrTags;
    vector<string> tags;

    explicit Photo(int id, bool isHorizontal, int nrTags, vector<string> tags) {
        this->id = id;
        this->isHorizontal = isHorizontal;
        this->nrTags = nrTags;
        this->tags = tags;
    }
};

class Slide {
public:
    vector<Photo*> photos;
    vector<string> tags;
    bool selected;

    explicit Slide(Photo* photo) {
        selected = false;
        photos.push_back(photo);
        for (int i = 0; i < photo->nrTags; i++) {
            tags.push_back(photo->tags[i]);
        }
    }

    explicit Slide(Photo* photo1, Photo* photo2) {
        selected = false;
        photos.push_back(photo1);
        for (int i = 0; i < photo1->nrTags; i++) {
            bool tagExists = false;

            for (int j = 0; j < tags.size(); j++) {
                if (tags[j] == photo1 -> tags[i]) {
                    tagExists = true;
                    break;
                }
            }

            if (!tagExists) {
                tags.push_back(photo1 -> tags[i]);
            }
        }

        photos.push_back(photo2);
        for (int i = 0; i < photo2->nrTags; i++) {
            bool tagExists = false;

            for (int j = 0; j < tags.size(); j++) {
                if (tags[j] == photo2 -> tags[i]) {
                    tagExists = true;
                    break;
                }
            }

            if (!tagExists) {
                tags.push_back(photo2 -> tags[i]);
            }
        }
    }
};


int minDiffSlides(Slide* slide1, Slide* slide2);
int DiffS1S2(Slide* slide1, Slide* slide2);
int DiffIntersect(Slide* slide1, Slide* slide2);
bool compareSlides(Slide* slide1, Slide* slide2);


int main() {
    ifstream fin("e_input.txt");
    ofstream fout("test.out");
    vector<Photo *> photos;

    int nrPhotos;
    char orientation;
    bool isHorizontal;
    int nrTags;
    vector<string> tags;
    string tag;
    Photo *photo;

    fin >> nrPhotos;

    for (int i = 0; i < nrPhotos; i++) {
        tags.clear();
        fin >> orientation;

        if (orientation == 'H') {
            isHorizontal = true;
        } else {
            isHorizontal = false;
        }

        fin >> nrTags;

        for (int j = 0; j < nrTags; j++) {
            fin >> tag;
            tags.push_back(tag);
        }

        photo = new Photo(i, isHorizontal, nrTags, tags);
        photos.push_back(photo);
    }

    // ----- VECTOR CU PHOTO GATA -----

    bool nextVerticalChoosen = false;
    vector<Slide*> slides;
    Slide* slide;

    for (int i = 0; i < nrPhotos; i++) {
        if (photos[i]->isHorizontal) {
            slide = new Slide(photos[i]);
            slides.push_back(slide);
        } else {
            if (!nextVerticalChoosen) {
                nextVerticalChoosen = true;
                for (int j = i+1; j < nrPhotos; j++) {
                    if (!photos[j]->isHorizontal) {
                        slide = new Slide(photos[i], photos[j]);
                        slides.push_back(slide);
                        break;
                    }
                }
            } else {
                nextVerticalChoosen = false;
            }
        }
    }

    // ----- VECTOR CU SLIDE-URI GATA -----

//    for (int i = 0; i < slides.size(); i++) {
//        for(int j = 0; j < slides[i]->tags.size(); j++) {
//            fout << slides[i]->tags[j] << " ";
//        }
//
//        fout << endl;
//    }
    fout << slides.size() << endl;

    sort(slides.begin(), slides.end(), compareSlides);


    int q=0;
    {
        for (int i = 0; i < slides[q]->photos.size(); i++) {
                fout << slides[q]->photos[i]->id << " ";
            }
            fout << endl;
    }
    slides[q]->selected=true;
   int aux=slides.size()-1;
    while(aux)
    {
       int min=-1;
        int imin=-1;
        int a = q + RAZA;
        if (a > slides.size()) {
            a = slides.size() - 1;
        }
        for(int i=q;i<a;i++)
        {
            if(minDiffSlides(slides[q],slides[i])>min&&!slides[i]->selected)
            {
                min=minDiffSlides(slides[q],slides[i]);
                imin=i;
            }
        }
        a=q-RAZA;
        if(a<0)
        a=0;
        for(int i=q;i>=a;i--)
        {
            if(minDiffSlides(slides[q],slides[i])>min&&!slides[i]->selected)
            {
                min=minDiffSlides(slides[q],slides[i]);
                imin=i;
            }
        }

        if (imin == -1) {
            for (int i = 0; i < slides.size(); i++) {
                if (!slides[i]->selected) {
                    q = i;
                    break;
                }
            }
        } else {
            q = imin;
        }

        slides[q]->selected=true;

        // for(int j = 0; j < slides[q]->tags.size(); j++) {
        //     fout << slides[q]->tags[j] << " ";
        // }

        // fout << endl;

        for (int j = 0; j < slides[q]->photos.size(); j++) {
                fout << slides[q]->photos[j]->id << " ";
            }
            fout << endl;

        aux--;
    }





    return 0;
}


int minDiffSlides(Slide* slide1, Slide* slide2) {
    int diffS1S2 = DiffS1S2(slide1, slide2);
    int diffS2S1 = DiffS1S2(slide2, slide1);
    int diffintersect = DiffIntersect(slide1, slide2);

    return min(min(diffS1S2, diffS2S1), diffintersect);
}

int DiffS1S2(Slide* slide1, Slide* slide2) {
    int k = 0;
    for(int i = 0; i < slide1->tags.size(); ++i) {
        int found = 0;
        for(int j = 0; j < slide2->tags.size(); ++j) {
            if(slide1->tags[i] == slide2->tags[j]) {
                found = 1;
                break;
            }
        }
        if(!found) {
            k++;
        }
    }
    return k;
}

int DiffIntersect(Slide* slide1, Slide* slide2) {
    int k = 0;
    for(int i = 0; i < slide1->tags.size(); ++i) {
        int found = 0;
        for(int j = 0; j < slide2->tags.size(); ++j) {
            if(slide1->tags[i] == slide2->tags[j]) {
                found = 1;
                break;
            }
        }
        if(found) {
            k++;
        }
    }
    return k;
}

bool compareSlides(Slide* slide1, Slide* slide2) {
    return (slide1->tags.size() < slide2->tags.size());
}
