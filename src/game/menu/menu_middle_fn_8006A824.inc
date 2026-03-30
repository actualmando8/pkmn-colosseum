    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r28, r3
    mr r29, r4
    lis r4, lbl_80267DD8@ha
    mr r3, r29
    addi r31, r4, lbl_80267DD8@l
    li r30, 0x1
    bl fn_8012A784
    clrlwi r0, r3, 24
    cmpwi r0, 0x1
    beq @L_8006A8B0
    bge @L_8006A878
    cmpwi r0, 0x0
    bge @L_8006A884
    b @L_8006A950
    @L_8006A878:
    cmpwi r0, 0x3
    bge @L_8006A950
    b @L_8006A900
    @L_8006A884:
    mr r3, r29
    li r30, 0x1
    bl fn_8012AA2C
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8006A960
    addi r3, r31, 0x10
    addi r5, r31, 0x20
    li r4, 0x281
    bl fn_80196E10
    b @L_8006A960
    @L_8006A8B0:
    mr r3, r29
    bl fn_8012AA2C
    clrlwi r0, r3, 24
    cmpwi r0, 0x1
    beq @L_8006A8E4
    bge @L_8006A8D4
    cmpwi r0, 0x0
    bge @L_8006A8DC
    b @L_8006A8EC
    @L_8006A8D4:
    cmpwi r0, 0x3
    b @L_8006A8EC
    @L_8006A8DC:
    li r30, 0x2
    b @L_8006A960
    @L_8006A8E4:
    li r30, 0x3
    b @L_8006A960
    @L_8006A8EC:
    addi r3, r31, 0x10
    addi r5, r31, 0x4c
    li r4, 0x28a
    bl fn_80196E10
    b @L_8006A960
    @L_8006A900:
    mr r3, r29
    bl fn_8012AA2C
    clrlwi r0, r3, 24
    cmpwi r0, 0x1
    beq @L_8006A934
    bge @L_8006A924
    cmpwi r0, 0x0
    bge @L_8006A92C
    b @L_8006A93C
    @L_8006A924:
    cmpwi r0, 0x3
    b @L_8006A93C
    @L_8006A92C:
    li r30, 0x309
    b @L_8006A960
    @L_8006A934:
    li r30, 0x308
    b @L_8006A960
    @L_8006A93C:
    addi r3, r31, 0x10
    addi r5, r31, 0x4c
    li r4, 0x294
    bl fn_80196E10
    b @L_8006A960
    @L_8006A950:
    addi r3, r31, 0x10
    addi r5, r31, 0x60
    li r4, 0x299
    bl fn_80196E10
    @L_8006A960:
    mr r3, r28
    mr r4, r29
    mr r5, r30
    bl fn_8006A990
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
