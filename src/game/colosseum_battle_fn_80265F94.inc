    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r4, 0xfa
    li r5, 0x66
    stw r0, 0x24(r1)
    li r6, 0x0
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r3
    li r3, 0x0
    stw r28, 0x10(r1)
    bl fn_8012640C
    clrlwi. r0, r3, 16
    lis r3, 0x1
    subi r3, r3, 0x1
    beq @L_80265FDC
    mr r3, r0
@L_80265FDC:
    li r4, 0x0
    li r5, 0x0
    bl fn_801DE190
    lhz r4, -20392(r2)
    mr r31, r3
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    mr r3, r31
    li r4, 0x1
    bl fn_801DA4E8
    lhz r4, -20392(r2)
    mr r3, r31
    li r5, 0x4
    mr r30, r4
    bl fn_801DA9E8
    lfs f1, -20388(r2)
    li r3, 0x2
    bl fn_801C41C8
    b @L_8026605C
@L_8026602C:
    bl fn_801DB088
    mr r3, r31
    mr r4, r30
    li r5, 0x4
    bl fn_801DA94C
    clrlwi. r0, r3, 24
    bne @L_80266058
    mr r3, r31
    mr r4, r30
    li r5, 0x4
    bl fn_801DA9E8
@L_80266058:
    bl fn_800F0308
@L_8026605C:
    li r3, 0x0
    bl fn_801C40F0
    extsb. r0, r3
    bne @L_8026602C
    li r3, 0xfa
    bl fn_8011E778
    cmplwi r3, 0x0
    bne @L_80266084
    li r28, 0x0
    b @L_802660C8
@L_80266084:
    bl fn_8011E15C
    clrlwi r28, r3, 16
    mr r3, r28
    bl fn_80166A28
    b @L_802660C8
@L_80266098:
    bl fn_801DB088
    mr r3, r31
    mr r4, r30
    li r5, 0x4
    bl fn_801DA94C
    clrlwi. r0, r3, 24
    bne @L_802660C4
    mr r3, r31
    mr r4, r30
    li r5, 0x4
    bl fn_801DA9E8
@L_802660C4:
    bl fn_800F0308
@L_802660C8:
    mr r3, r28
    bl fn_801666BC
    cmpwi r3, 0x2
    beq @L_80266098
    li r3, 0x44ba
    li r4, 0x0
    li r5, 0x0
    bl fn_801067E8
    b @L_8026611C
@L_802660EC:
    bl fn_801DB088
    mr r3, r31
    mr r4, r30
    li r5, 0x4
    bl fn_801DA94C
    clrlwi. r0, r3, 24
    bne @L_80266118
    mr r3, r31
    mr r4, r30
    li r5, 0x4
    bl fn_801DA9E8
@L_80266118:
    bl fn_800F0308
@L_8026611C:
    bl fn_80106934
    extsb r0, r3
    subfic r0, r0, 0x1
    cntlzw r0, r0
    extrwi. r0, r0, 8, 19
    bne @L_802660EC
    li r3, 0x1
    bl fn_801065B8
    cmpwi r29, 0x1
    beq @L_80266168
    bge @L_8026617C
    cmpwi r29, 0x0
    bge @L_80266154
    b @L_8026617C
@L_80266154:
    li r3, 0x5d
    li r4, 0x3d2
    bl fn_80132A38
    li r3, 0x44bc
    b @L_80266180
@L_80266168:
    li r3, 0x5d
    li r4, 0x3d2
    bl fn_80132A38
    li r3, 0x44bb
    b @L_80266180
@L_8026617C:
    li r3, 0x44b9
@L_80266180:
    li r4, 0x0
    li r5, 0x0
    bl fn_801067E8
    b @L_802661C0
@L_80266190:
    bl fn_801DB088
    mr r3, r31
    mr r4, r30
    li r5, 0x4
    bl fn_801DA94C
    clrlwi. r0, r3, 24
    bne @L_802661BC
    mr r3, r31
    mr r4, r30
    li r5, 0x4
    bl fn_801DA9E8
@L_802661BC:
    bl fn_800F0308
@L_802661C0:
    bl fn_80106934
    extsb r0, r3
    subfic r0, r0, 0x1
    cntlzw r0, r0
    extrwi. r0, r0, 8, 19
    bne @L_80266190
    li r3, 0x1
    bl fn_801065B8
    lfs f1, -20388(r2)
    li r3, 0x3
    bl fn_801C41C8
    b @L_80266220
@L_802661F0:
    bl fn_801DB088
    mr r3, r31
    mr r4, r30
    li r5, 0x4
    bl fn_801DA94C
    clrlwi. r0, r3, 24
    bne @L_8026621C
    mr r3, r31
    mr r4, r30
    li r5, 0x4
    bl fn_801DA9E8
@L_8026621C:
    bl fn_800F0308
@L_80266220:
    li r3, 0x0
    bl fn_801C40F0
    extsb. r0, r3
    bne @L_802661F0
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
