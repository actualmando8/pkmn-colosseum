    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    lis r3, -32728
    stw r0, 0x44(r1)
    stw r31, 0x3c(r1)
    stw r30, 0x38(r1)
    addi r30, r3, -27848
    stw r29, 0x34(r1)
    bl fn_800057A8
    cmpwi r3, 0x2
    bne @L_801D5754
    lwz r11, 0x48(r30)
    addi r3, r1, 0x20
    lwz r10, 0x4c(r30)
    lwz r9, 0x50(r30)
    lwz r8, 0x54(r30)
    lwz r7, 0x58(r30)
    lwz r6, 0x5c(r30)
    lwz r5, 0x60(r30)
    lwz r4, 0x64(r30)
    lwz r0, 0x68(r30)
    stw r11, 0x20(r1)
    stw r10, 0x24(r1)
    stw r9, 0x28(r1)
    stw r8, 0x14(r1)
    stw r7, 0x18(r1)
    stw r6, 0x1c(r1)
    stw r5, 0x8(r1)
    stw r4, 0xc(r1)
    stw r0, 0x10(r1)
    bl fn_801778DC
    addi r3, r1, 0x14
    bl fn_80177930
    addi r3, r1, 0x8
    bl fn_80177984
    li r3, 0x2
    bl fn_80177A44
    lis r3, -32698
    li r0, 0x0
    stw r0, 29688(r3)
    b @L_801D5768
@L_801D5754:
    li r3, 0x7
    bl fn_80177A44
    lis r3, -32698
    li r0, 0x1
    stw r0, 29688(r3)
@L_801D5768:
    lis r3, -32698
    li r4, 0x0
    addi r3, r3, 29688
    li r5, 0x4
    addi r3, r3, 0x8
    bl memset
    lis r3, -32698
    li r5, 0x0
    addi r4, r3, 29688
    li r3, -0x1
    lwz r0, 0x624(r4)
    stw r5, 0x14(r4)
    rlwinm r0, r0, 2, 0, 29
    stw r3, 0x18(r4)
    add r3, r4, r0
    lwz r3, 0x61c(r3)
    cmplwi r3, 0x0
    beq @L_801D57C0
    lwz r3, 0x24(r3)
    cmplwi r3, 0x0
    beq @L_801D57C0
    bl fn_800E5D40
@L_801D57C0:
    lis r3, -32698
    li r0, 0x0
    addi r3, r3, 29688
    stw r0, 0x65c(r3)
    stw r0, 0x664(r3)
    stw r0, 0xc(r3)
    bl DVDInit
    lis r3, -32698
    lwzu r0, 29688(r3)
    li r4, 0x0
    cmpwi r0, 0x1
    stw r4, 0x63c(r3)
    beq @L_801D587C
    bge @L_801D588C
    cmpwi r0, 0x0
    bge @L_801D5804
    b @L_801D588C
@L_801D5804:
    addi r29, r3, 0x61c
    lwz r3, 0x61c(r3)
    bl fn_801DB100
    lis r3, -32698
    addi r30, r3, 29688
    lhz r3, 0x660(r30)
    cmplwi r3, 0x0
    beq @L_801D5860
    bl fn_800E24B0
    lhz r3, 0x660(r30)
    bl fn_800E209C
    lis r3, -32698
    li r0, 0x0
    addi r31, r3, 29688
    sth r0, 0x660(r30)
    lhz r3, 0x668(r31)
    cmplwi r3, 0x0
    beq @L_801D5860
    bl fn_800E24B0
    lhz r3, 0x668(r31)
    bl fn_800E209C
    li r0, 0x0
    sth r0, 0x668(r31)
@L_801D5860:
    mr r3, r29
    li r4, 0x0
    li r5, 0x8
    bl memset
    li r3, 0x0
    bl fn_801D5A94
    b @L_801D588C
@L_801D587C:
    addi r3, r3, 0x61c
    li r4, 0x0
    li r5, 0x8
    bl memset
@L_801D588C:
    lis r3, -32698
    addi r3, r3, 29688
    addi r3, r3, 0x21c
    bl strlen
    cmplwi r3, 0x0
    beq @L_801D58A8
    bl fn_801D5DA0
@L_801D58A8:
    bl fn_800057A8
    cmpwi r3, 0x2
    bne @L_801D58C0
    li r3, 0x2
    bl fn_80177A44
    b @L_801D58C8
@L_801D58C0:
    li r3, 0x7
    bl fn_80177A44
@L_801D58C8:
    lwz r0, 0x44(r1)
    lwz r31, 0x3c(r1)
    lwz r30, 0x38(r1)
    lwz r29, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
