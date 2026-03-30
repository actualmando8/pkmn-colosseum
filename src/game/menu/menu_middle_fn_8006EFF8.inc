    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r22, 0x8(r1)
    mr r31, r3
    li r4, 0x0
    bl fn_801040D0
    mr r28, r3
    mr r3, r31
    li r4, 0xa40
    bl fn_801046C8
    lbz r0, 0x94(r31)
    extsb r4, r0
    neg r0, r4
    andc r0, r0, r4
    srwi r4, r0, 31
    bl fn_80109220
    mr r3, r31
    li r4, 0xa3f
    bl fn_801046C8
    lbz r0, 0x94(r31)
    li r5, 0x32
    extsb r0, r0
    xor r0, r5, r0
    srawi r4, r0, 1
    and r0, r0, r5
    subf r0, r0, r4
    srwi r4, r0, 31
    bl fn_80109220
    lis r3, lbl_80268424@ha
    li r25, 0x0
    addi r27, r3, lbl_80268424@l
    @L_8006F078:
    lhz r4, 0x0(r27)
    mr r3, r31
    bl fn_801046C8
    lhz r4, 0x2(r27)
    mr r29, r3
    mr r3, r31
    bl fn_801046C8
    lhz r4, 0x4(r27)
    mr r30, r3
    mr r3, r31
    bl fn_801046C8
    lhz r4, 0x6(r27)
    mr r24, r3
    mr r3, r31
    bl fn_801046C8
    lbz r0, 0x94(r31)
    mr r23, r3
    extsb r0, r0
    add r26, r25, r0
    cmpwi r26, 0x3c
    bne @L_8006F0E0
    li r3, 0x43e6
    li r0, 0x0
    stw r3, 0x4c(r29)
    stw r0, 0x4c(r30)
    b @L_8006F164
    @L_8006F0E0:
    mr r3, r26
    bl fn_80077D88
    mr r22, r3
    bl fn_80142984
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8006F110
    mr r3, r22
    bl fn_801440A0
    bl fn_80144088
    stw r3, 0x4c(r29)
    b @L_8006F118
    @L_8006F110:
    li r0, 0x12e
    stw r0, 0x4c(r29)
    @L_8006F118:
    lbzx r0, r28, r26
    cmplwi r0, 0x0
    beq @L_8006F148
    beq @L_8006F130
    li r0, 0x3d6f
    b @L_8006F134
    @L_8006F130:
    li r0, 0x3d68
    @L_8006F134:
    lis r3, 0xff00
    stw r0, 0x4c(r30)
    addi r0, r3, 0xff
    stw r0, 0x64(r30)
    b @L_8006F164
    @L_8006F148:
    beq @L_8006F154
    li r0, 0x3d6f
    b @L_8006F158
    @L_8006F154:
    li r0, 0x3d68
    @L_8006F158:
    stw r0, 0x4c(r30)
    li r0, -0x1
    stw r0, 0x64(r30)
    @L_8006F164:
    lbzx r4, r28, r26
    mr r3, r24
    bl fn_80109220
    lbz r0, 0x95(r31)
    mr r3, r23
    extsb r0, r0
    subf r0, r25, r0
    cntlzw r0, r0
    srwi r0, r0, 5
    clrlwi r4, r0, 24
    bl fn_80109220
    addi r27, r27, 0x8
    addi r25, r25, 0x1
    cmplwi r25, 0xb
    blt @L_8006F078
    lbz r0, 0x1(r31)
    extsb r0, r0
    cmpwi r0, 0x3
    beq @L_8006F224
    bge @L_8006F1C0
    cmpwi r0, 0x0
    beq @L_8006F1C8
    b @L_8006F270
    @L_8006F1C0:
    cmpwi r0, 0x6
    b @L_8006F270
    @L_8006F1C8:
    lbz r0, 0x2(r31)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_8006F270
    lis r3, lbl_80267EA8@ha
    lwz r24, 0x1c(r31)
    addi r3, r3, lbl_80267EA8@l
    lhz r23, 0x18(r3)
    b @L_8006F204
    @L_8006F1EC:
    lha r0, 0x6(r24)
    mr r3, r31
    mr r5, r23
    clrlwi r4, r0, 16
    bl fn_801081F8
    lwz r24, 0x0(r24)
    @L_8006F204:
    cmplwi r24, 0x0
    bne @L_8006F1EC
    mr r3, r31
    li r23, 0x424a
    li r4, 0xe4c
    bl fn_801046C8
    stw r23, 0x4c(r3)
    b @L_8006F270
    @L_8006F224:
    lbz r0, 0x2(r31)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_8006F270
    lis r3, lbl_80267EA8@ha
    lwz r24, 0x1c(r31)
    addi r3, r3, lbl_80267EA8@l
    lhz r23, 0x1a(r3)
    b @L_8006F260
    @L_8006F248:
    lha r0, 0x6(r24)
    mr r3, r31
    mr r5, r23
    clrlwi r4, r0, 16
    bl fn_801081F8
    lwz r24, 0x0(r24)
    @L_8006F260:
    cmplwi r24, 0x0
    bne @L_8006F248
    li r0, 0x1
    stb r0, 0x2(r31)
    @L_8006F270:
    lmw r22, 0x8(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
