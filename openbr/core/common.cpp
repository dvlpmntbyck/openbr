/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright 2012 The MITRE Corporation                                      *
 *                                                                           *
 * Licensed under the Apache License, Version 2.0 (the "License");           *
 * you may not use this file except in compliance with the License.          *
 * You may obtain a copy of the License at                                   *
 *                                                                           *
 *     http://www.apache.org/licenses/LICENSE-2.0                            *
 *                                                                           *
 * Unless required by applicable law or agreed to in writing, software       *
 * distributed under the License is distributed on an "AS IS" BASIS,         *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  *
 * See the License for the specific language governing permissions and       *
 * limitations under the License.                                            *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "common.h"
#include <QMutex>

using namespace std;

/**** GLOBAL ****/
void Common::seedRNG() {
    static QMutex seedControl;
    QMutexLocker lock(&seedControl);

    static bool seeded = false;
    if (!seeded) {
        srand(0); // We seed with 0 instead of time(NULL) to have reproducible randomness
        seeded = true;
    }
}

QList<int> Common::RandSample(int n, int max, int min, bool unique)
{
    QList<int> samples; samples.reserve(n);
    int range = max-min;
    if (range <= 0) qFatal("Non-positive range.");
    if (unique && (n >= range)) {
        for (int i=min; i<max; i++)
            samples.append(i);
        return samples;
    }

    while (samples.size() < n) {
        const int sample = (rand() % range) + min;
        if (unique && samples.contains(sample)) continue;
        samples.append(sample);
    }
    return samples;
}

QList<int> Common::RandSample(int n, const QSet<int> &values, bool unique)
{
    QList<int> valueList = values.toList();
    if (unique && (values.size() <= n)) return valueList;

    QList<int> samples; samples.reserve(n);
    while (samples.size() < n) {
        const int randIndex = rand() % valueList.size();
        samples.append(valueList[randIndex]);
        if (unique) valueList.removeAt(randIndex);
    }
    return samples;
}
