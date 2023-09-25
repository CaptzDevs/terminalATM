int sortByTel(const void* a, const void* b) {
    return strcmp(((User*)a)->tel, ((User*)b)->tel);
}

int sortByID(const void* a, const void* b) {
    return strcmp(((User*)a)->id, ((User*)b)->id);
}

int sortByAccountID(const void* a, const void* b) {
    return strcmp(((User*)a)->accountID, ((User*)b)->accountID);
}