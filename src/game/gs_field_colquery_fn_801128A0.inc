    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    lis r3, lbl_80272094@ha
    lis r4, fn_801129CC@ha
    addi r8, r3, lbl_80272094@l
    lwz r7, 0x0(r8)
    addi r5, r4, fn_801129CC@l
    lwz r6, 0x4(r8)
    li r3, 0x0
    lwz r0, 0x8(r8)
    li r4, 0x5000
    stw r7, 0x8(r1)
    stw r6, 0xc(r1)
    stw r0, 0x10(r1)
    bl fn_800FF3C0
    lis r4, fn_801129AC@ha
    li r3, 0xff
    addi r5, r4, fn_801129AC@l
    li r4, 0x5000
    bl fn_800FF178
    addi r3, r1, 0x8
    li r4, 0x1
    bl fn_800FF4D4
    addi r3, r1, 0x8
    li r4, 0x2
    bl fn_800FF4D4
    bl fn_800057A8
    cmpwi r3, 0x3
    beq @L_80112960
    bge @L_80112930
    cmpwi r3, 0x1
    beq @L_80112950
    bge @L_80112958
    b @L_80112960
@L_80112930:
    cmpwi r3, 0x5
    beq @L_80112940
    bge @L_80112960
    b @L_80112948
@L_80112940:
    li r31, 0x3e4
    b @L_80112964
@L_80112948:
    li r31, 0x320
    b @L_80112964
@L_80112950:
    li r31, 0x3e6
    b @L_80112964
@L_80112958:
    li r31, 0x3e7
    b @L_80112964
@L_80112960:
    li r31, 0x399
@L_80112964:
    lis r3, lbl_80408378@ha
    li r5, 0x0
    addi r4, r3, lbl_80408378@l
    li r0, 0x1
    stw r5, 0x0(r4)
    mr r3, r31
    stw r31, 0x4(r4)
    stb r0, 0x8(r4)
    stw r5, 0xc(r4)
    bl fn_800FF788
    mr r3, r31
    bl fn_800FF784
    bl fn_80118020
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
